/*
 * File: gdsparse.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSParse class which is used to parse a GDS file and create a
 * GDSObjects object containing the contents of the file according to the 
 * process configuration.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <cmath>
#include <cstring>

#include "gdsparse.h"

extern int verbose_output;

GDSParse::GDSParse (GDSConfig *config, GDSProcess *process, bool generate_process) :
	m_libname(""), m_topcellname(""),
	m_currentlayer(-1), m_currentdatatype(-1),  m_currentwidth(0.0),
	m_currentpathtype(0),
	m_currenttexttype(0), m_currentpresentation(0),
	m_textstring(""), m_currentstrans(0),
	m_currentangle(0.0), m_currentmag(1.0),
	m_currentbgnextn(0.0), m_currentendextn(0.0),
	m_sname(""), m_arrayrows(0), m_arraycols(0),
	m_units(0.0), m_angle(0.0),
	m_iptr(NULL), m_optr(NULL),

	m_process(process), m_config(config),
	m_boundary(NULL),
	m_recordlen(0),

	m_bounding_output(false), m_use_outfile(false), m_allow_multiple_output(false),
	m_output_children_first(false), m_generate_process(generate_process),

	m_pathelements(0), m_boundaryelements(0), m_boxelements(0),
	m_textelements(0), m_srefelements(0), m_arefelements(0),
	m_currentobject(NULL)
{

	for(int i=0; i<70; i++){
		m_unsupported[i] = false;
	}
	for(int i=0; i<256; i++){
		for(int j=0; j<256; j++){
			m_layer_warning[i][j] = false;
		}
	}

}

GDSParse::~GDSParse ()
{
	while(!m_objects.empty()){
		delete m_objects[m_objects.size()-1];
		m_objects.pop_back();
	}

	if(m_boundary){
		delete m_boundary;
	}
}

bool GDSParse::Parse(FILE *iptr)
{
	m_iptr = iptr;
	if(m_iptr){
		//DEBUG
		//printf("GDSParse::Parse(%p)\n",iptr);

		bool result = ParseFile();

		v_printf(1, "\nSumary:\n\tPaths:\t\t%ld\n\tBoundaries:\t%ld\n\tBoxes:\t\t%ld\n\tStrings:\t%ld\n\tStuctures:\t%ld\n\tArrays:\t\t%ld\n",
			m_pathelements, m_boundaryelements, m_boxelements, m_textelements, m_srefelements, m_arefelements);

		return result;
	}else{
		return -1;
	}
}

void GDSParse::AssignASRefs(void)
{
	/* Assign objects to srefs and arefs */
	/* FIXME - surely there is a better way than 3n^3 loop */
	for(unsigned int i = 0; i < m_objects.size(); i++){
		GDSObject *object = m_objects[i];

		for(unsigned int j = 0; j < object->GetSRefCount(); j++){
			ASRefElement *sref = object->GetSRef(j);

			if(sref){
				for(unsigned int k = 0; k < m_objects.size(); k++){
					if(sref->name == m_objects[k]->GetName()){
						sref->object = m_objects[k];
						break;
					}
				}
			}else{
				break;
			}
		}

		for(unsigned int j = 0; j < object->GetARefCount(); j++){
			ASRefElement *aref = object->GetARef(j);

			if(aref){
				for(unsigned int k = 0; k < m_objects.size(); k++){
					if(aref->name == m_objects[k]->GetName()){
						aref->object = m_objects[k];
						break;
					}
				}
			}else{
				break;
			}
		}
	}
}

void GDSParse::Output(FILE *optr, std::string topcell)
{
	m_topcellname = topcell;

	AssignASRefs();

	if(m_use_outfile){
		m_optr = optr;
	}
	if(m_optr || !m_use_outfile){
		OutputHeader();

		if(!m_bounding_output){
			long objectid = 0;
			if(topcell.length() > 0){
				RecursiveOutput(GetObjectRef(topcell), m_optr, 0.0, 0.0, &objectid);
			}else{
				RecursiveOutput(m_objects[0], m_optr, 0.0, 0.0, &objectid);
			}
		}

		OutputFooter();
	}
}

void GDSParse::RecursiveOutput(GDSObject *object, FILE *m_optr, float offx, float offy, long *objectid)
{
	if(!object){
		return;
	}

	if(object->GetIsOutput() && m_allow_multiple_output == false){
		return;
	}

	if(m_output_children_first && object->HasASRef()){
		GDSObject *child;

		for(unsigned int i = 0; i < object->GetSRefCount(); i++){
			child = object->GetSRef(i)->object;
			if(child && (child != object)){
				RecursiveOutput(child, m_optr, offx, offy, objectid);
			}
		}

		for(unsigned int i = 0; i < object->GetARefCount(); i++){
			child = object->GetARef(i)->object;
			if(child && (child != object)){
				RecursiveOutput(child, m_optr, offx, offy, objectid);
			}
		}

	}

	ProcessLayer *layer = NULL;
	if(m_process){
		layer = m_process->GetLayer();
	}

	object->OutputToFile(m_optr, m_config->GetFont(), offx, offy, objectid, layer);
}

bool GDSParse::ParseFile()
{
	byte recordtype, datatype;
	char *tempstr;
	ProcessLayer *layer;

	if(!m_iptr){
		return -1;
	}

	fseek(m_iptr, 0, SEEK_SET);
	while(!feof(m_iptr)){
		m_recordlen = GetTwoByteSignedInt();
		fread(&recordtype, 1, 1, m_iptr);
		fread(&datatype, 1, 1, m_iptr);
		m_recordlen -= 4;
		switch(recordtype){
			case rnHeader:
				v_printf(2, "HEADER\n");
				ParseHeader();
				break;
			case rnBgnLib:
				v_printf(2, "BGNLIB\n");
				while(m_recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnLibName:
				v_printf(2, "LIBNAME ");
				ParseLibName();
				break;
			case rnUnits:
				v_printf(2, "UNITS\n");
				ParseUnits();
				break;
			case rnEndLib:
				v_printf(2, "ENDLIB\n");
				fseek(m_iptr, 0, SEEK_END);
				return 0;
				break;
			case rnEndStr:
				v_printf(2, "ENDSTR\n");
				break;
			case rnEndEl:
				v_printf(2, "ENDEL\n\n");
				/* Empty, no need to parse */
				break;
			case rnBgnStr:
				v_printf(2, "BGNSTR\n");
				while(m_recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnStrName:
				v_printf(2, "STRNAME ");
				ParseStrName();
				break;
			case rnBoundary:
				v_printf(2, "BOUNDARY ");
				m_currentelement = elBoundary;
				break;
			case rnPath:
				v_printf(2, "PATH ");
				m_currentelement = elPath;
				break;
			case rnSRef:
				v_printf(2, "SREF ");
				m_currentelement = elSRef;
				break;
			case rnARef:
				v_printf(2, "AREF ");
				m_currentelement = elARef;
				break;
			case rnText:
				v_printf(2, "TEXT ");
				m_currentelement = elText;
				break;
			case rnLayer:
				m_currentlayer = GetTwoByteSignedInt();
				v_printf(2, "LAYER (%d)\n", m_currentlayer);
				break;
			case rnDataType:
				m_currentdatatype = GetTwoByteSignedInt();
				v_printf(2, "DATATYPE (%d)\n", m_currentdatatype);
				break;
			case rnWidth:
				m_currentwidth = (float)(GetFourByteSignedInt()/2);
				if(m_currentwidth > 0){
					m_currentwidth *= m_units;
				}
				v_printf(2, "WIDTH (%.3f)\n", m_currentwidth*2);
				// Scale to a half to make width correct when adding and
				// subtracting
				break;
			case rnXY:
				v_printf(2, "XY ");
				switch(m_currentelement){
					case elBoundary:
						m_boundaryelements++;
						ParseXYBoundary();
						break;
					case elBox:
						m_boxelements++;
						ParseXYBoundary();
						break;
					case elPath:
						m_pathelements++;
						ParseXYPath();
						break;
					default:
						ParseXY();
						break;
				}
				break;
			case rnColRow:
				m_arraycols = GetTwoByteSignedInt();
				m_arrayrows = GetTwoByteSignedInt();
				v_printf(2, "COLROW (Columns = %d Rows = %d)\n", m_arraycols, m_arrayrows);
				break;
			case rnSName:
				ParseSName();
				break;
			case rnPathType:
				if(!m_unsupported[rnPathType]){
					v_printf(1, "Incomplete support for GDS2 record type: PATHTYPE\n");
					m_unsupported[rnPathType] = true;
				}
				//FIXME
				m_currentpathtype = GetTwoByteSignedInt();
				v_printf(2, "PATHTYPE (%d)\n", m_currentpathtype);
				break;
			case rnTextType:
				ReportUnsupported("TEXTTYPE", rnTextType);
				m_currenttexttype = GetTwoByteSignedInt();
				v_printf(2, "TEXTTYPE (%d)\n", m_currenttexttype);
				break;
			case rnPresentation:
				m_currentpresentation = GetTwoByteSignedInt();
				v_printf(2, "PRESENTATION (%d)\n", m_currentpresentation);
				break;
			case rnString:
				v_printf(2, "STRING ");
				tempstr = GetAsciiString();
				if(tempstr){
					m_textstring = tempstr;
					delete [] tempstr;
				}
				/* Only set string if the current object is valid, the text string is valid 
				 * and we are using a layer that is defined and being shown.
				 */
				if(m_currentobject && m_currentobject->GetCurrentText() && m_textstring.length() > 0){
					if(m_process != NULL){
						layer = m_process->GetLayer(m_currentlayer, m_currentdatatype);
						if(layer && layer->Show){
							m_currentobject->GetCurrentText()->SetString(m_textstring);
						}
					}else{
						m_currentobject->GetCurrentText()->SetString(m_textstring);
					}
					v_printf(2, "(\"%s\")", m_textstring.c_str());
				}else if(m_textstring.length() == 0){
					return -1;
				}
				v_printf(2, "\n");
				break;
			case rnSTrans:
				if(!m_unsupported[rnSTrans]){
					v_printf(1, "Incomplete support for GDS2 record type: STRANS\n");
					m_unsupported[rnSTrans] = true;
				}
				//FIXME
				m_currentstrans = GetTwoByteSignedInt();
				v_printf(2, "STRANS (%d)\n", m_currentstrans);
				break;
			case rnMag:
				m_currentmag = GetEightByteReal();
				v_printf(2, "MAG (%f)\n", m_currentmag);
				break;
			case rnAngle:
				m_currentangle = (float)GetEightByteReal();
				v_printf(2, "ANGLE (%f)\n", m_currentangle);
				break;
/*			case rnUInteger:
				break;
Not used in GDS2 spec	case rnUString:
				break;
*/
			case rnRefLibs:
				ReportUnsupported("REFLIBS", rnRefLibs);
				tempstr = GetAsciiString();
				v_printf(2, "REFLIBS (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnFonts:
				ReportUnsupported("FONTS", rnFonts);
				tempstr = GetAsciiString();
				v_printf(2, "FONTS (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnGenerations:
				ReportUnsupported("GENERATIONS", rnGenerations);
				v_printf(2, "GENERATIONS\n");
				v_printf(2, "\t");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, "\n");
				break;
			case rnAttrTable:
				ReportUnsupported("ATTRTABLE", rnAttrTable);
				tempstr = GetAsciiString();
				v_printf(2, "ATTRTABLE (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnStypTable:
				ReportUnsupported("STYPTABLE", rnStypTable);
				v_printf(2, "STYPTABLE (\"%d\")\n", GetTwoByteSignedInt());
				break;
			case rnStrType:
				ReportUnsupported("STRTYPE", rnStrType);
				tempstr = GetAsciiString();
				v_printf(2, "STRTYPE (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnElFlags:
				ReportUnsupported("ELFLAGS", rnElFlags);
				v_printf(2, "ELFLAGS (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnElKey:
				ReportUnsupported("ELKEY", rnElKey);
				v_printf(2, "ELKEY (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnLinkType:
				ReportUnsupported("LINKTYPE", rnLinkType);
				v_printf(2, "LINKTYPE (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnLinkKeys:
				ReportUnsupported("LINKKEYS", rnLinkKeys);
				v_printf(2, "LINKKEYS (");
				while(m_recordlen){
					v_printf(2, "%ld ", GetFourByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnNodeType:
				ReportUnsupported("NODETYPE", rnNodeType);
				v_printf(2, "NODETYPE (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnPropAttr:
				ReportUnsupported("PROPATTR", rnPropAttr);
				v_printf(2, "PROPATTR (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnPropValue:
				ReportUnsupported("PROPVALUE", rnPropValue);
				tempstr = GetAsciiString();
				v_printf(2, "PROPVALUE (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnBox:
				ReportUnsupported("BOX", rnBox);
				v_printf(2, "BOX\n");
				/* Empty */
				m_currentelement = elBox;
				break;
			case rnBoxType:
				ReportUnsupported("BOXTYPE", rnBoxType);
				v_printf(2, "BOXTYPE (%d)\n", GetTwoByteSignedInt());
				break;
			case rnPlex:
				ReportUnsupported("PLEX", rnPlex);
				v_printf(2, "PLEX (");
				while(m_recordlen){
					v_printf(2, "%ld ", GetFourByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnBgnExtn:
				ReportUnsupported("BGNEXTN", rnBgnExtn);
				m_currentbgnextn = m_units * (float)GetFourByteSignedInt();
				v_printf(2, "BGNEXTN (%f)\n", m_currentbgnextn);
				break;
			case rnEndExtn:
				ReportUnsupported("ENDEXTN", rnEndExtn);
				m_currentendextn = m_units * (float)GetFourByteSignedInt();
				v_printf(2, "ENDEXTN (%ld)\n", m_currentendextn);
				break;
			case rnTapeNum:
				ReportUnsupported("TAPENUM", rnTapeNum);
				v_printf(2, "TAPENUM\n");
				v_printf(2, "\t");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, "\n");
				break;
			case rnTapeCode:
				ReportUnsupported("TAPECODE", rnTapeCode);
				v_printf(2, "TAPECODE\n");
				v_printf(2, "\t");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, "\n");
				break;
			case rnStrClass:
				ReportUnsupported("STRCLASS", rnStrClass);
				v_printf(2, "STRCLASS (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnReserved:
				ReportUnsupported("RESERVED", rnReserved);
				v_printf(2, "RESERVED\n");
				/* Empty */
				break;
			case rnFormat:
				ReportUnsupported("FORMAT", rnFormat);
				v_printf(2, "FORMAT (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnMask:
				ReportUnsupported("MASK", rnMask);
				tempstr = GetAsciiString();
				v_printf(2, "MASK (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnEndMasks:
				ReportUnsupported("ENDMASKS", rnEndMasks);
				v_printf(2, "ENDMASKS\n");
				/* Empty */
				break;
			case rnLibDirSize:
				ReportUnsupported("LIBDIRSIZE", rnLibDirSize);
				v_printf(2, "LIBDIRSIZE (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnSrfName:
				ReportUnsupported("SRFNAME", rnSrfName);
				tempstr = GetAsciiString();
				v_printf(2, "SRFNAME (\"%s\")\n", tempstr);
				delete [] tempstr;
				break;
			case rnLibSecur:
				ReportUnsupported("LIBSECUR", rnLibSecur);
				v_printf(2, "LIBSECUR (");
				while(m_recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnBorder:
				ReportUnsupported("BORDER", rnBorder);
				v_printf(2, "BORDER\n");
				/* Empty */
				break;
			case rnSoftFence:
				ReportUnsupported("SOFTFENCE", rnSoftFence);
				v_printf(2, "SOFTFENCE\n");
				/* Empty */
				break;
			case rnHardFence:
				ReportUnsupported("HARDFENCE", rnHardFence);
				v_printf(2, "HARDFENCE\n");
				/* Empty */
				break;
			case rnSoftWire:
				ReportUnsupported("SOFTWIRE", rnSoftWire);
				v_printf(2, "SOFTWIRE\n");
				/* Empty */
				break;
			case rnHardWire:
				ReportUnsupported("HARDWIRE", rnHardWire);
				v_printf(2, "HARDWIRE\n");
				/* Empty */
				break;
			case rnPathPort:
				ReportUnsupported("PATHPORT", rnPathPort);
				v_printf(2, "PATHPORT\n");
				/* Empty */
				break;
			case rnNodePort:
				ReportUnsupported("NODEPORT", rnNodePort);
				v_printf(2, "NODEPORT\n");
				/* Empty */
				break;
			case rnUserConstraint:
				ReportUnsupported("USERCONSTRAINT", rnUserConstraint);
				v_printf(2, "USERCONSTRAINT\n");
				/* Empty */
				break;
			case rnSpacerError:
				ReportUnsupported("SPACERERROR", rnSpacerError);
				v_printf(2, "SPACERERROR\n");
				/* Empty */
				break;
			case rnContact:
				ReportUnsupported("CONTACT", rnContact);
				v_printf(2, "CONTACT\n");
				/* Empty */
				break;
			default:
				v_printf(1, "Unknown record type (%d) at position %ld.", recordtype, ftell(m_iptr));

				return -1;
				break;
	
		}
	}	
	return 0;
}

void GDSParse::ParseHeader()
{
	short version;
	version = GetTwoByteSignedInt();
	v_printf(2, "\tVersion = %d\n", version);
}

void GDSParse::ParseLibName()
{
	char *str;
	str = GetAsciiString();
	m_libname = str;
	v_printf(2, " (\"%s\")\n", m_libname.c_str());
	delete [] str;
}

void GDSParse::ParseSName()
{
	v_printf(2, "SNAME ");

	char *str;
	str = GetAsciiString();
	m_sname = str;
	for(unsigned int i=0; i<m_sname.length(); i++){
		if(m_sname[i] && (m_sname[i] < 48 || m_sname[i] > 57) && (m_sname[i] < 65 || m_sname[i] > 90) && (m_sname[i] < 97 || m_sname[i] > 122)){
			m_sname[i] = '_';
		}
	}
	v_printf(2, "(\"%s\")\n", m_sname.c_str());
	delete [] str;
}

void GDSParse::ParseUnits()
{
	double tmp;
	m_units = (float)GetEightByteReal() * m_config->GetScale(); 
	tmp = GetEightByteReal();
	v_printf(1, "DB units/user units = %g\nSize of DB units in metres = %g\nSize of user units in m = %g\n\n", 1/m_units, tmp, tmp/m_units);
}

void GDSParse::ParseStrName()
{
	char *str = GetAsciiString();

	if(str){
		// Disallow invalid characters in POV-Ray _names.
		for(unsigned int i=0; i<strlen(str); i++){
			if(str[i] && (str[i] < 48 || str[i] > 57) && (str[i] < 65 || str[i] > 90) && (str[i] < 97 || str[i] > 122)){
				str[i] = '_';
			}
		}
		v_printf(2, "(\"%s\")", str);

		// This calls our own NewObject function which is pure virtual so the end 
		// user must define it. This means we can always add a unknown object as
		// long as it inherits from GDSObject.
		m_objects.push_back(NewObject(str));
		m_currentobject = m_objects[m_objects.size()-1];
		delete [] str;
	}
	v_printf(2, "\n");
}

void GDSParse::ParseXYPath()
{
	float X, Y;
	int points = m_recordlen/8;
	int i;
	ProcessLayer *thislayer = NULL;

	if(m_process != NULL){
		thislayer = m_process->GetLayer(m_currentlayer, m_currentdatatype);

		if(thislayer==NULL){
			// m_layer_warning only has fixed bounds at the moment.
			// Not sure how to best make it dynamic.

			if(!m_generate_process){
				if(m_currentlayer == -1 || m_currentdatatype == -1 || !m_layer_warning[m_currentlayer][m_currentdatatype]){
					v_printf(1, "Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", m_currentlayer, m_currentdatatype);
					v_printf(1, "\tIgnoring this layer.\n");
					m_layer_warning[m_currentlayer][m_currentdatatype] = true;
				}
			}else{
				if(!m_layer_warning[m_currentlayer][m_currentdatatype]){
					m_process->AddLayer(m_currentlayer, m_currentdatatype);
					m_layer_warning[m_currentlayer][m_currentdatatype] = true;
				}
			}
			while(m_recordlen){
				GetFourByteSignedInt();
			}
			m_currentwidth = 0.0; // Always reset to default for paths in case width not specified
			m_currentpathtype = 0;
			m_currentangle = 0.0;
			m_currentdatatype = -1;
			m_currentmag = 1.0;
			return;
		}
	}

	if(m_currentwidth){
		/* FIXME - need to check for -ve value and then not scale */
		if(thislayer && thislayer->Thickness && thislayer->Show && m_currentobject){
			m_currentobject->AddPath(m_currentpathtype, m_units*thislayer->Height, m_units*thislayer->Thickness, points, m_currentwidth, m_currentbgnextn, m_currentendextn, thislayer);
		}
		for(i=0; i<points; i++){
			X = m_units * (float)GetFourByteSignedInt();
			Y = m_units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f) ", X, Y);
			if(thislayer && thislayer->Thickness && thislayer->Show && m_currentobject){
				m_currentobject->GetCurrentPath()->AddPoint(i, X, Y);
			}
		}
	}else{
		for(i=0; i<points; i++){
			GetFourByteSignedInt();
			GetFourByteSignedInt();
		}
	}
	v_printf(2, "\n");
	m_currentwidth = 0.0; // Always reset to default for paths in case width not specified
	m_currentpathtype = 0;
	m_currentangle = 0.0;
	m_currentdatatype = -1;
	m_currentmag = 1.0;
	m_currentbgnextn = 0.0;
	m_currentendextn = 0.0;
}


void GDSParse::ParseXYBoundary()
{
	float X, Y;
	float firstX=0.0, firstY=0.0;
	int points = m_recordlen/8;
	int i;
	ProcessLayer *thislayer = NULL;

	if(m_process != NULL){
		thislayer = m_process->GetLayer(m_currentlayer, m_currentdatatype);

		if(thislayer==NULL){
			if(!m_generate_process){
				if(m_currentlayer == -1 || m_currentdatatype == -1 || !m_layer_warning[m_currentlayer][m_currentdatatype]){
					v_printf(1, "Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", m_currentlayer, m_currentdatatype);
					v_printf(1, "\tIgnoring this layer.\n");
					m_layer_warning[m_currentlayer][m_currentdatatype] = true;
				}
			}else{
				if(!m_layer_warning[m_currentlayer][m_currentdatatype]){
					m_process->AddLayer(m_currentlayer, m_currentdatatype);
					m_layer_warning[m_currentlayer][m_currentdatatype] = true;
				}
			}
			while(m_recordlen){
				GetFourByteSignedInt();
			}
			m_currentwidth = 0.0; // Always reset to default for paths in case width not specified
			m_currentpathtype = 0;
			m_currentangle = 0.0;
			m_currentdatatype = -1;
			m_currentmag = 1.0;
			return;
		}
	}

	if(thislayer && thislayer->Thickness && thislayer->Show && m_currentobject){
		//FIXME - why was this points+1 ? m_currentobject->AddPolygon(m_units*thislayer->Height, m_units*thislayer->Thickness, points+1, thislayer->Name);
		m_currentobject->AddPolygon(m_units*thislayer->Height, m_units*thislayer->Thickness, points, thislayer);
	}

	for(i=0; i<points; i++){
		X = m_units * (float)GetFourByteSignedInt();
		Y = m_units * (float)GetFourByteSignedInt();
		v_printf(2, "(%.3f,%.3f) ", X, Y);
		if(i==0){
			firstX = X;
			firstY = Y;
		}
		if(thislayer && thislayer->Thickness && thislayer->Show && m_currentobject){
			m_currentobject->GetCurrentPolygon()->AddPoint(i, X, Y);
		}
	}
	v_printf(2, "\n");
	if(thislayer && thislayer->Thickness && thislayer->Show && m_currentobject){
		m_currentobject->GetCurrentPolygon()->AddPoint(i, firstX, firstY);
		//m_currentobject->GetCurrentPolygon()->SetColour(thislayer->Red, thislayer->Green, thislayer->Blue, thislayer->Filter, thislayer->Metal);
	}
	m_currentwidth = 0.0; // Always reset to default for paths in case width not specified
	m_currentpathtype = 0;
	m_currentangle = 0.0;
	m_currentdatatype = -1;
	m_currentmag = 1.0;
	m_currentbgnextn = 0.0;
	m_currentendextn = 0.0;
}

void GDSParse::ParseXY()
{
	float X, Y;
	float firstX=0.0, firstY=0.0, secondX=0.0, secondY=0.0;
	ProcessLayer *thislayer = NULL;
	bool Flipped;

	if(m_process != NULL){
		thislayer = m_process->GetLayer(m_currentlayer, m_currentdatatype);
	}
	Flipped = ((u_int16_t)(m_currentstrans & 0x8000) == (u_int16_t)0x8000) ? true : false;

	switch(m_currentelement){
		case elSRef:
			m_srefelements++;
			X = m_units * (float)GetFourByteSignedInt();
			Y = m_units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f)\n", X, Y);

			if(m_currentobject){
				m_currentobject->AddSRef(m_sname, X, Y, Flipped, m_currentmag);
				if(m_currentangle){
					m_currentobject->SetSRefRotation(0, -m_currentangle, 0);
				}
			}
			break;

		case elARef:
			m_arefelements++;
			firstX = m_units * (float)GetFourByteSignedInt();
			firstY = m_units * (float)GetFourByteSignedInt();
			secondX = m_units * (float)GetFourByteSignedInt();
			secondY = m_units * (float)GetFourByteSignedInt();
			X = m_units * (float)GetFourByteSignedInt();
			Y = m_units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f) ", firstX, firstY);
			v_printf(2, "(%.3f,%.3f) ", secondX, secondY);
			v_printf(2, "(%.3f,%.3f)\n", X, Y);

			if(m_currentobject){
				m_currentobject->AddARef(m_sname, firstX, firstY, secondX, secondY, X, Y, m_arraycols, m_arrayrows, Flipped, m_currentmag);
				if(m_currentangle){
					m_currentobject->SetARefRotation(0, -m_currentangle, 0);
				}
			}
			break;

		case elText:
			m_textelements++;

			if(thislayer==NULL){
				if(!m_generate_process){
					v_printf(2, "Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", m_currentlayer, m_currentdatatype);
					v_printf(2, "\tIgnoring this string.\n");
				}else{
					if(!m_layer_warning[m_currentlayer][m_currentdatatype]){
						m_process->AddLayer(m_currentlayer, m_currentdatatype);
						m_layer_warning[m_currentlayer][m_currentdatatype] = true;
					}
				}
				while(m_recordlen){
					GetFourByteSignedInt();
				}
				m_currentwidth = 0.0; // Always reset to default for paths in case width not specified
				m_currentpathtype = 0;
				m_currentangle = 0.0;
				m_currentdatatype = 0;
				m_currentmag = 1.0;
				return;
			}

			X = m_units * (float)GetFourByteSignedInt();
			Y = m_units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f)\n", X, Y);

			if(m_currentobject && m_currentobject->GetCurrentText()){
				int vert_just, horiz_just;

				vert_just = (((((unsigned long)m_currentpresentation & 0x8 ) == (unsigned long)0x8 ) ? 2 : 0) + (((((unsigned long)m_currentpresentation & 0x4 ) == (unsigned long)0x4 ) ? 1 : 0)));
				horiz_just = (((((unsigned long)m_currentpresentation & 0x2 ) == (unsigned long)0x2 ) ? 2 : 0) + (((((unsigned long)m_currentpresentation & 0x1 ) == (unsigned long)0x1 ) ? 1 : 0)));

				m_currentobject->AddText(X, Y, m_units*thislayer->Height, Flipped, m_currentmag, vert_just, horiz_just, thislayer);
				if(m_currentangle){
					m_currentobject->GetCurrentText()->SetRotation(0.0, -m_currentangle, 0.0);
				}
			}
			break;
		default:
			while(m_recordlen){
				GetFourByteSignedInt();
			}
			break;
	}
	m_currentwidth = 0.0; // Always reset to default for paths in case width not specified
	m_currentpathtype = 0;
	m_currentangle = 0.0;
	m_currentdatatype = -1;
	m_currentmag = 1.0;
	m_currentpresentation = 0;
}

short GDSParse::GetBitArray()
{
	byte byte1;

	fread(&byte1, 1, 1, m_iptr);
	fread(&byte1, 1, 1, m_iptr);

	m_recordlen-=2;
	return 0;
}

double GDSParse::GetEightByteReal()
{
	byte value;
	byte b8, b2, b3, b4, b5, b6, b7;
	double sign=1.0;
	double exponent;
	double mant;

	fread(&value, 1, 1, m_iptr);
	if(value & 128){
		value -= 128;
		sign = -1.0;
	}
	exponent = (double )value;
	exponent -= 64.0;
	mant=0.0;

	fread(&b2, 1, 1, m_iptr);
	fread(&b3, 1, 1, m_iptr);
	fread(&b4, 1, 1, m_iptr);
	fread(&b5, 1, 1, m_iptr);
	fread(&b6, 1, 1, m_iptr);
	fread(&b7, 1, 1, m_iptr);
	fread(&b8, 1, 1, m_iptr);

	mant += b8;
	mant /= 256.0;
	mant += b7;
	mant /= 256.0;
	mant += b6;
	mant /= 256.0;
	mant += b5;
	mant /= 256.0;
	mant += b4;
	mant /= 256.0;
	mant += b3;
	mant /= 256.0;
	mant += b2;
	mant /= 256.0;

	m_recordlen-=8;

	return sign*(mant*pow(16.0,exponent));
}

int32_t GDSParse::GetFourByteSignedInt()
{
	int32_t value;
	fread(&value, 4, 1, m_iptr);
	
	m_recordlen-=4;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	return endian_swap_long(value);
#else
	return value;
#endif
}

int16_t GDSParse::GetTwoByteSignedInt()
{
	int16_t value;

	fread(&value, 2, 1, m_iptr);

	m_recordlen-=2;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	return endian_swap_short(value);
#else
	return value;
#endif
}

char *GDSParse::GetAsciiString()
{
	char *str=NULL;
	
	if(m_recordlen>0){
		m_recordlen += m_recordlen%2; /* Make sure length is even */
		str = new char[m_recordlen+1];
		if(!str){
			fprintf(stderr, "Unable to allocate memory for ascii string (%d)\n", m_recordlen);
			return NULL;
		}
		fread(str, 1, m_recordlen, m_iptr);
		str[m_recordlen] = 0;
		m_recordlen = 0;
	}
	return str;
}

void GDSParse::ReportUnsupported(std::string name, enum RecordNumbers rn)
{
	if(!m_unsupported[rn]){
		v_printf(1, "Unsupported GDS2 record type: %s\n", name.c_str());
		m_unsupported[rn] = true;
	}

}


struct _Boundary *GDSParse::GetBoundary()
{
	if(!m_boundary){
		m_boundary = new struct _Boundary;
	}

	m_boundary->xmax = m_boundary->ymax = -10000000.0;
	m_boundary->xmin = m_boundary->ymin =  10000000.0;

	for(unsigned int i = 0; i < m_objects.size(); i++){
		struct _Boundary *object_bound = m_objects[i]->GetBoundary();

		if(object_bound->xmax > m_boundary->xmax){
			m_boundary->xmax = object_bound->xmax;
		}
		if(object_bound->xmin < m_boundary->xmin){
			m_boundary->xmin = object_bound->xmin;
		}
		if(object_bound->ymax > m_boundary->ymax){
			m_boundary->ymax = object_bound->ymax;
		}
		if(object_bound->ymin < m_boundary->ymin){
			m_boundary->ymin = object_bound->ymin;
		}
	}
	return m_boundary;
}

GDSObject *GDSParse::GetObjectRef(std::string name)
{
	if(!m_objects.empty() && name.length() > 0){	
		for(unsigned int i = 0; i < m_objects.size(); i++){
			if(name == m_objects[i]->GetName()){
				return m_objects[i];
			}
		}
	}
	return NULL;
}


