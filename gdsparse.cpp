#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdsparse.h"
#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"
#include "gdstext.h"
#include "gdspolygon.h"

extern int verbose_output;

GDSParse::GDSParse (char *infile, char *outfile, char *configfile, char *processfile, char *topcell)
{
	topcellname = topcell;

	iptr = NULL;
	optr = NULL;
	libname = NULL;
	sname = NULL;
	textstring = NULL;
	Objects = NULL;

	PathElements = 0;
	BoundaryElements = 0;
	TextElements = 0;
	SRefElements = 0;
	ARefElements = 0;

	currentangle = 0.0;
	currentwidth = 0.0;
	currentstrans = 0;
	currentpathtype = 0;
	currentdatatype = -1;
	currentmag = 1.0;
	currentbgnextn = 0.0;
	currentendextn = 0.0;
	currenttexttype = 0;
	currentpresentation = 0;
	currentlayer = 0;
//	currentelement = 0;
	arrayrows = 0;
	arraycols = 0;
	angle = 0.0;
	units = 0.0;
	recordlen = 0;
	CurrentObject = NULL;

	process = NULL;
	config = NULL;

	for(int i=0; i<70; i++){
		unsupported[i] = false;
	}

	if(configfile){
		config = new GDSConfig(configfile);
	}else{
		config = new GDSConfig(); // Start with default positions
	}
	if(!config->IsValid()){
		return;
	}
	//if(){
	//	printf("
	// return'
	//}

	/* 
	** Order of precedence for process.txt:
	** -p switch (given as an argument to this function)
	** Specified in config file
	** Use process.txt if none others specified.
	*/
	if(processfile == NULL){
		if(config->GetProcessFile()!=NULL){
			processfile = config->GetProcessFile();
		}else{
			processfile = new char[13];
			strncpy(processfile, "process.txt", strlen("process.txt")+1);
		}
	}

	process = new GDSProcess(processfile);
	if(!process->IsValid()){
		fprintf(stderr, "Error: Invalid process file\n");
		return;
	}else if(process->LayerCount()==0){
		fprintf(stderr, "Error: No layers found in \"%s\".\n", processfile);
		return;
	}


	iptr = fopen(infile, "rb");
	if(iptr){
		optr = fopen(outfile, "wb");
		if(optr){
			Objects = new GDSObjects;
			Parse();

			switch(output_format){
				case opPOV:
					OutputPOVHeader();
					break;
				case op3DS:
					Output3DSHeader();
					break;
				case opOGL:
					OutputOGLHeader();
					break;
			}

			if(!bounding_output){
				long objectid = 0;
				if(topcellname){
					if(output_format == opOGL){
#ifdef OGLVIEWER
						gl_main(Objects->GetObject(topcell));
#else
						RecursiveOutput(Objects->GetObject(topcell), optr, 0.0, 0.0, &objectid);
#endif
					}else{
						RecursiveOutput(Objects->GetObject(topcell), optr, 0.0, 0.0, &objectid);
						if(output_format == opPOV){
							fprintf(optr, "object { str_%s }\n", topcell);
						}
					}
				}else{
					if(output_format == opOGL){
#ifdef OGLVIEWER
						gl_main(Objects->GetObject(0));
#else
						RecursiveOutput(Objects->GetObject(0), optr, 0.0, 0.0, &objectid);
#endif
					}else{
						RecursiveOutput(Objects->GetObject(0), optr, 0.0, 0.0, &objectid);
						if(output_format == opPOV){
							fprintf(optr, "object { str_%s }\n", Objects->GetObject(0)->GetName());
						}
					}
				}
			}
			switch(output_format){
				case opPOV:
					break;
				case op3DS:
					Output3DSFooter();
					break;
				case opOGL:
#ifndef OGLVIEWER
					OutputOGLFooter();
#endif
					break;
			}
			fclose(optr);
			optr = NULL;
		}
		fclose(iptr);
		iptr = NULL;

		v_printf(1, "\nSummary:\n\tPaths:\t\t%ld\n\tBoundaries:\t%ld\n\tStrings:\t%ld\n\tStuctures:\t%ld\n\tArrays:\t\t%ld\n",
			PathElements, BoundaryElements, TextElements, SRefElements, ARefElements);
	}else{
		fprintf(stderr, "Unable to read %s\n", infile);
	}
}

GDSParse::~GDSParse ()
{
	if(config){
		delete config;
	}
	if(process){
		delete process;
	}
	if(iptr){
		fclose(iptr);
	}
	if(optr){
		fclose(optr);
	}
	if(Objects){
		delete Objects;
	}
}

bool GDSParse::Parse(char *infile, char *topcell)
{
	iptr = fopen(infile, "rb");
	if(iptr){
		Objects = new GDSObjects;
		Parse();

		switch(output_format){
			case opPOV:
				OutputPOVHeader();
				break;
			case op3DS:
				Output3DSHeader();
				break;
			case opOGL:
				OutputOGLHeader();
				break;
		}

		if(!bounding_output){
			long objectid = 0;
			if(topcellname){
				if(output_format == opOGL){
					//gl_main(Objects->GetObject(topcell));
					RecursiveOutput(Objects->GetObject(topcell), optr, 0.0, 0.0, &objectid);
				}else{
					RecursiveOutput(Objects->GetObject(topcell), optr, 0.0, 0.0, &objectid);
					if(output_format == opPOV){
						fprintf(optr, "object { str_%s }\n", topcell);
					}
				}
			}else{
				if(output_format == opOGL){
				//	gl_main(Objects->GetObject(0));
					RecursiveOutput(Objects->GetObject(0), optr, 0.0, 0.0, &objectid);
				}else{
					RecursiveOutput(Objects->GetObject(0), optr, 0.0, 0.0, &objectid);
					if(output_format == opPOV){
						fprintf(optr, "object { str_%s }\n", Objects->GetObject(0)->GetName());
					}
				}
			}
		}
		switch(output_format){
			case opPOV:
				break;
			case op3DS:
				Output3DSFooter();
				break;
			case opOGL:
				OutputOGLFooter();
				break;
		}
		fclose(optr);
		optr = NULL;
	}
	fclose(iptr);
	iptr = NULL;

	v_printf(1, "\nSummary:\n\tPaths:\t\t%ld\n\tBoundaries:\t%ld\n\tStrings:\t%ld\n\tStuctures:\t%ld\n\tArrays:\t\t%ld\n",
		PathElements, BoundaryElements, TextElements, SRefElements, ARefElements);
//}else{
	fprintf(stderr, "Unable to read %s\n", infile);
//}
}

void GDSParse::RecursiveOutput(class GDSObject *Object, FILE *optr, float offx, float offy, long *objectid)
{
	if(!Object || Object->GetIsOutput()){
		if(output_format != opOGL){
			return;
		}
	}
	if(Object->HasASRef()){
		GDSObject *child;

		int i=0;
		do{
			child = Object->GetSRef(Objects, i);
			if(child && (child != Object)){
				RecursiveOutput(child, optr, offx, offy, objectid);
			}

			i++;
		}while(child);

		i = 0;
		do{
			child = Object->GetARef(Objects, i);
			if(child && (child != Object)){
				RecursiveOutput(child, optr, offx, offy, objectid);
			}
			i++;
		}while(child);

	}
	switch(output_format){
		case opPOV:
			Object->OutputToPOV(optr, Objects, config->GetFont());
			break;
		case op3DS:
			Object->OutputTo3DS(optr, Objects, config->GetFont(), offx, offy, objectid, process->GetLayer());
			break;
		case opOGL:
			Object->OutputToOGL(optr, Objects, config->GetFont(), offx, offy, process);
			break;
	}
}

void GDSParse::OutputPOVHeader()
{
	if(optr){
		fprintf(optr, "#include \"colors.inc\"\n");
		fprintf(optr, "#include \"metals.inc\"\n");
		fprintf(optr, "#include \"transforms.inc\"\n");

		struct _Boundary *Boundary = Objects->GetBoundary();
		float half_widthX = (Boundary->XMax - Boundary->XMin)/2;
		float half_widthY = (Boundary->YMax - Boundary->YMin)/2;
		float centreX = half_widthX + Boundary->XMin;
		float centreY = half_widthY + Boundary->YMin;

		float distance;
		if(half_widthX > half_widthY){
			distance = half_widthX * 1.8;
		}else{
			distance = half_widthY * 1.8;
		}


		fprintf(optr, "// TopLeft: %.2f, %.2f\n", Boundary->XMin, Boundary->YMax);
		fprintf(optr, "// TopRight: %.2f, %.2f\n", Boundary->XMax, Boundary->YMax);
		fprintf(optr, "// BottomLeft: %.2f, %.2f\n", Boundary->XMin, Boundary->YMin);
		fprintf(optr, "// BottomRight: %.2f, %.2f\n", Boundary->XMax, Boundary->YMin);
		fprintf(optr, "// Centre: %.2f, %.2f\n", centreX, centreY);

		float XMod = config->GetCameraPos()->XMod;
		float YMod = config->GetCameraPos()->YMod;
		float ZMod = config->GetCameraPos()->ZMod;

		switch(config->GetCameraPos()->boundarypos){
			case bpCentre:
				// Default camera angle = 67.38
				// Half of this is 33.69
				// tan(33.69) = 0.66666 = 1/1.5
				// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8

				fprintf(optr, "camera {\n\tlocation <%.2f,%.2f,%.2f>\n", centreX*XMod, centreY*YMod, -distance*ZMod);
				break;
			case bpTopLeft:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpTopRight:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpBottomLeft:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
			case bpBottomRight:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
		}

		fprintf(optr, "\tsky <0,0,-1>\n"); //This fixes the look at rotation (hopefully)

		XMod = config->GetLookAtPos()->XMod;
		YMod = config->GetLookAtPos()->YMod;
		ZMod = config->GetLookAtPos()->ZMod;

		switch(config->GetLookAtPos()->boundarypos){
			case bpCentre:
				fprintf(optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", centreX*XMod, centreY*YMod, -distance*ZMod);
				break;
			case bpTopLeft:
				fprintf(optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMin*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpTopRight:
				fprintf(optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMax*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpBottomLeft:
				fprintf(optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMin*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
			case bpBottomRight:
				fprintf(optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMax*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
		}

		if(config->GetLightPos()!=NULL){
			Position dummypos;
			dummypos.Next = config->GetLightPos();

			Position *LightPos = &dummypos;

			while(LightPos->Next){
				LightPos = LightPos->Next;
				XMod = LightPos->XMod;
				YMod = LightPos->YMod;
				ZMod = LightPos->ZMod;

				switch(LightPos->boundarypos){
					case bpCentre:
						fprintf(optr, "light_source {<%.2f,%.2f,%.2f> White }\n", centreX*XMod, centreY*YMod, -distance*ZMod);
						break;
					case bpTopLeft:
						fprintf(optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMin*XMod, Boundary->YMax*YMod, -distance*ZMod);
						break;
					case bpTopRight:
						fprintf(optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMax*XMod, Boundary->YMax*YMod, -distance*ZMod);
						break;
					case bpBottomLeft:
						fprintf(optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMin*XMod, Boundary->YMin*YMod, -distance*ZMod);
						break;
					case bpBottomRight:
						fprintf(optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMax*XMod, Boundary->YMin*YMod, -distance*ZMod);
						break;
				}
			}
		}else{
			fprintf(optr, "light_source {<%.2f,%.2f,%.2f> White }\n", centreX, centreY, -distance);
		}

		fprintf(optr, "background { color Black }\n");
		fprintf(optr, "global_settings { ambient_light rgb <%.2f,%.2f,%.2f> }\n", config->GetAmbient(), config->GetAmbient(), config->GetAmbient());

		/* Output layer texture information */
		struct ProcessLayer *firstlayer;
		firstlayer = process->GetLayer();
		while(firstlayer->Next){
			if(firstlayer->Show){
				if(!firstlayer->Metal){
					fprintf(optr, "#declare t%s = pigment{rgbf <%.2f, %.2f, %.2f, %.2f>}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
				}else{
					fprintf(optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
				}
			}
			firstlayer = firstlayer->Next;
		}
		if(firstlayer->Show){
			if(!firstlayer->Metal){
				fprintf(optr, "#declare t%s = pigment{rgbf <%.2f, %.2f, %.2f, %.2f>}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
			}else{
				fprintf(optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
			}
		}

		if(bounding_output){
			fprintf(optr, "box {<%.2f,%.2f,%.2f> <%.2f,%.2f,%.2f> texture { pigment { rgb <0.75, 0.75, 0.75> } } }", Boundary->XMin, Boundary->YMin,units*process->GetLowest(),Boundary->XMax, Boundary->YMax,units*process->GetHighest());
		}
	}
}

void GDSParse::Parse()
{
	byte recordtype, datatype;
	char *tempstr;

	if(!iptr){
		return;
	}

	fseek(iptr, 0, SEEK_SET);
	while(!feof(iptr)){
		recordlen = GetTwoByteSignedInt();
		fread(&recordtype, 1, 1, iptr);
		fread(&datatype, 1, 1, iptr);
		recordlen -= 4;
		switch(recordtype){
			case rnHeader:
				v_printf(2, "HEADER\n");
				ParseHeader();
				break;
			case rnBgnLib:
				v_printf(2, "BGNLIB\n");
				while(recordlen){
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
				fseek(iptr, 0, SEEK_END);
				return;
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
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnStrName:
				v_printf(2, "STRNAME ");
				ParseStrName();
				break;
			case rnBoundary:
				v_printf(2, "BOUNDARY ");
				currentelement = elBoundary;
				break;
			case rnPath:
				v_printf(2, "PATH ");
				currentelement = elPath;
				break;
			case rnSRef:
				v_printf(2, "SREF ");
				currentelement = elSRef;
				break;
			case rnARef:
				v_printf(2, "AREF ");
				currentelement = elARef;
				break;
			case rnText:
				v_printf(2, "TEXT ");
				currentelement = elText;
				break;
			case rnLayer:
				currentlayer = GetTwoByteSignedInt();
				v_printf(2, "LAYER (%d)\n", currentlayer);
				break;
			case rnDataType:
				currentdatatype = GetTwoByteSignedInt();
				v_printf(2, "DATATYPE (%d)\n", currentdatatype);
				break;
			case rnWidth:
				currentwidth = (float)(GetFourByteSignedInt()/2);
				if(currentwidth > 0){
					currentwidth *= units;
				}
				v_printf(2, "WIDTH (%.3f)\n", currentwidth*2);
				// Scale to a half to make width correct when adding and
				// subtracting
				break;
			case rnXY:
				v_printf(2, "XY ");
				switch(currentelement){
					case elBoundary:
						BoundaryElements++;
						ParseXYBoundary();
						break;
					case elPath:
						PathElements++;
						ParseXYPath();
						break;
					default:
						ParseXY();
						break;
				}
				break;
			case rnColRow:
				arraycols = GetTwoByteSignedInt();
				arrayrows = GetTwoByteSignedInt();
				v_printf(2, "COLROW (Columns = %d Rows = %d)\n", arraycols, arrayrows);
				break;
			case rnSName:
				ParseSName();
				break;
			case rnPathType:
				if(!unsupported[rnPathType]){
					v_printf(1, "Incomplete support for GDS2 record type: PATHTYPE\n");
					unsupported[rnPathType] = true;
				}
				//FIXME
				currentpathtype = GetTwoByteSignedInt();
				v_printf(2, "PATHTYPE (%d)\n", currentpathtype);
				break;
			case rnTextType:
				ReportUnsupported("TEXTTYPE", rnTextType);
				currenttexttype = GetTwoByteSignedInt();
				v_printf(2, "TEXTTYPE (%d)\n", currenttexttype);
				break;
			case rnPresentation:
				currentpresentation = GetTwoByteSignedInt();
				v_printf(2, "PRESENTATION (%d)\n", currentpresentation);
				break;
			case rnString:
				v_printf(2, "STRING ");
				if(textstring){
					delete textstring;
					textstring = NULL;
				}
				textstring = GetAsciiString();
				if(CurrentObject && textstring){
					CurrentObject->GetCurrentText()->SetString(textstring);
					v_printf(2, "(\"%s\")", textstring);
					delete textstring;
					textstring = NULL;
				}
				v_printf(2, "\n");
				break;
			case rnSTrans:
				if(!unsupported[rnSTrans]){
					v_printf(1, "Incomplete support for GDS2 record type: STRANS\n");
					unsupported[rnSTrans] = true;
				}
				//FIXME
				currentstrans = GetTwoByteSignedInt();
				v_printf(2, "STRANS (%d)\n", currentstrans);
				break;
			case rnMag:
				currentmag = GetEightByteReal();
				v_printf(2, "MAG (%f)\n", currentmag);
				break;
			case rnAngle:
				currentangle = (float)GetEightByteReal();
				v_printf(2, "ANGLE (%f)\n", currentangle);
				break;
/*			case rnUInteger:
				break;
Not Used	case rnUString:
				break;
*/
			case rnRefLibs:
				ReportUnsupported("REFLIBS", rnRefLibs);
				tempstr = GetAsciiString();
				v_printf(2, "REFLIBS (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnFonts:
				ReportUnsupported("FONTS", rnFonts);
				tempstr = GetAsciiString();
				v_printf(2, "FONTS (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnGenerations:
				ReportUnsupported("GENERATIONS", rnGenerations);
				v_printf(2, "GENERATIONS\n");
				v_printf(2, "\t");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, "\n");
				break;
			case rnAttrTable:
				ReportUnsupported("ATTRTABLE", rnAttrTable);
				tempstr = GetAsciiString();
				v_printf(2, "ATTRTABLE (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnStypTable:
				ReportUnsupported("STYPTABLE", rnStypTable);
				tempstr = GetAsciiString();
				v_printf(2, "STYPTABLE (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnStrType:
				ReportUnsupported("STRTYPE", rnStrType);
				tempstr = GetAsciiString();
				v_printf(2, "STRTYPE (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnElFlags:
				ReportUnsupported("ELFLAGS", rnElFlags);
				v_printf(2, "ELFLAGS (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnElKey:
				ReportUnsupported("ELKEY", rnElKey);
				v_printf(2, "ELKEY (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnLinkType:
				ReportUnsupported("LINKTYPE", rnLinkType);
				v_printf(2, "LINKTYPE (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnLinkKeys:
				ReportUnsupported("LINKKEYS", rnLinkKeys);
				v_printf(2, "LINKKEYS (");
				while(recordlen){
					v_printf(2, "%ld ", GetFourByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnNodeType:
				ReportUnsupported("NODETYPE", rnNodeType);
				v_printf(2, "NODETYPE (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnPropAttr:
				ReportUnsupported("PROPATTR", rnPropAttr);
				v_printf(2, "PROPATTR (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnPropValue:
				ReportUnsupported("PROPVALUE", rnPropValue);
				tempstr = GetAsciiString();
				v_printf(2, "PROPVALUE (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnBox:
				ReportUnsupported("BOX", rnBox);
				v_printf(2, "BOX\n");
				/* Empty */
				break;
			case rnBoxType:
				ReportUnsupported("BOXTYPE", rnBoxType);
				v_printf(2, "BOXTYPE (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnPlex:
				ReportUnsupported("PLEX", rnPlex);
				v_printf(2, "PLEX (");
				while(recordlen){
					v_printf(2, "%ld ", GetFourByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnBgnExtn:
				ReportUnsupported("BGNEXTN", rnBgnExtn);
				currentbgnextn = GetFourByteSignedInt();
				v_printf(2, "BGNEXTN (%f)\n", currentbgnextn);
				break;
			case rnEndExtn:
				ReportUnsupported("ENDEXTN", rnEndExtn);
				currentendextn = GetFourByteSignedInt();
				v_printf(2, "ENDEXTN (%ld)\n", currentendextn);
				break;
			case rnTapeNum:
				ReportUnsupported("TAPENUM", rnTapeNum);
				v_printf(2, "TAPENUM\n");
				v_printf(2, "\t");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, "\n");
				break;
			case rnTapeCode:
				ReportUnsupported("TAPECODE", rnTapeCode);
				v_printf(2, "TAPECODE\n");
				v_printf(2, "\t");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, "\n");
				break;
			case rnStrClass:
				ReportUnsupported("STRCLASS", rnStrClass);
				v_printf(2, "STRCLASS (");
				while(recordlen){
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
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnMask:
				ReportUnsupported("MASK", rnMask);
				tempstr = GetAsciiString();
				v_printf(2, "MASK (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnEndMasks:
				ReportUnsupported("ENDMASKS", rnEndMasks);
				v_printf(2, "ENDMASKS\n");
				/* Empty */
				break;
			case rnLibDirSize:
				ReportUnsupported("LIBDIRSIZE", rnLibDirSize);
				v_printf(2, "LIBDIRSIZE (");
				while(recordlen){
					v_printf(2, "%d ", GetTwoByteSignedInt());
				}
				v_printf(2, ")\n");
				break;
			case rnSrfName:
				ReportUnsupported("SRFNAME", rnSrfName);
				tempstr = GetAsciiString();
				v_printf(2, "SRFNAME (\"%s\")\n", tempstr);
				delete tempstr;
				break;
			case rnLibSecur:
				ReportUnsupported("LIBSECUR", rnLibSecur);
				v_printf(2, "LIBSECUR (");
				while(recordlen){
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
				v_printf(1, "Unknown record type (%d) at position %ld.", recordtype, ftell(iptr));

				return;
				break;
	
		}
	}	
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
	if(libname){
		delete libname;
		libname = NULL;
	}
	libname = new char[strlen(str)+1];
	if(libname){
		strcpy(libname, str);
		v_printf(2, " (\"%s\")\n", libname);
	}else{
		fprintf(stderr, "\nUnable to allocate memory for string (%d)\n", strlen(str)+1);
	}
	delete str;
}

void GDSParse::ParseSName()
{
	v_printf(2, "SNAME ");

	char *str;
	str = GetAsciiString();
	if(sname){
		delete sname;
		sname = NULL;
	}
	sname = new char[strlen(str)+1];
	if(sname){
		strcpy(sname, str);
		for(unsigned int i=0; i<strlen(sname); i++){
			if(sname[i] && (sname[i] < 48 || sname[i] > 57) && (sname[i] < 65 || sname[i] > 90) && (sname[i] < 97 || sname[i] > 122)){
				sname[i] = '_';
			}
		}
		v_printf(2, "(\"%s\")\n", sname);
	}else{
		fprintf(stderr, "Unable to allocate memory for string (%d)\n", strlen(str)+1);
	}
	delete str;
}

void GDSParse::ParseUnits()
{
	double tmp;
	units = (float)GetEightByteReal();
	tmp = GetEightByteReal();
	v_printf(1, "DB units/user units = %g\nSize of DB units in metres = %g\nSize of user units in m = %g\n\n", 1/units, tmp, tmp/units);
}

void GDSParse::ParseStrName()
{
	char *str=NULL;
	str = GetAsciiString();

	if(str){
		// Disallow invalid characters in POV-Ray names.
		for(unsigned int i=0; i<strlen(str); i++){
			if(str[i] && (str[i] < 48 || str[i] > 57) && (str[i] < 65 || str[i] > 90) && (str[i] < 97 || str[i] > 122)){
				str[i] = '_';
			}
		}
		v_printf(2, "(\"%s\")", str);

		CurrentObject = Objects->AddObject(str);
		delete str;
	}
	v_printf(2, "\n");
}

void GDSParse::ParseXYPath()
{
	float X, Y;
	int points = recordlen/8;
	int i;
	struct ProcessLayer *thislayer;

	thislayer = process->GetLayer(currentlayer, currentdatatype);

	if(thislayer==NULL){
		v_printf(1, "Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
		v_printf(1, "\tIgnoring this path.\n");
		while(recordlen){
			GetFourByteSignedInt();
		}
		currentwidth = 0.0; // Always reset to default for paths in case width not specified
		currentpathtype = 0;
		currentangle = 0.0;
		currentdatatype = -1;
		currentmag = 1.0;
		return;
	}

	if(currentwidth){
		/* FIXME - need to check for -ve value and then not scale */
		if(thislayer->Height && thislayer->Show && CurrentObject){
			CurrentObject->AddPath(currentpathtype, units*thislayer->Height, units*thislayer->Thickness, points, currentwidth, currentbgnextn, currentendextn, thislayer);
		}
		for(i=0; i<points; i++){
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f) ", X, Y);
			if(thislayer->Height && thislayer->Show && CurrentObject){
				CurrentObject->GetCurrentPath()->AddPoint(i, X, Y);
			}
		}
	}
	v_printf(2, "\n");
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
	currentdatatype = -1;
	currentmag = 1.0;
	currentbgnextn = 0.0;
	currentendextn = 0.0;
}

void GDSParse::ParseXYBoundary()
{
	float X, Y;
	float firstX=0.0, firstY=0.0;
	int points = recordlen/8;
	int i;
	struct ProcessLayer *thislayer;

	thislayer = process->GetLayer(currentlayer, currentdatatype);

	if(thislayer==NULL){
		v_printf(1, "Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
		v_printf(1, "\tIgnoring this boundary.\n");
		while(recordlen){
			GetFourByteSignedInt();
		}
		currentwidth = 0.0; // Always reset to default for paths in case width not specified
		currentpathtype = 0;
		currentangle = 0.0;
		currentdatatype = -1;
		currentmag = 1.0;
		return;
	}

	if(thislayer->Height && thislayer->Show && CurrentObject){
		//FIXME - why was this points+1 ? CurrentObject->AddPolygon(units*thislayer->Height, units*thislayer->Thickness, points+1, thislayer->Name);
		CurrentObject->AddPolygon(units*thislayer->Height, units*thislayer->Thickness, points, thislayer);
	}

	for(i=0; i<points; i++){
		X = units * (float)GetFourByteSignedInt();
		Y = units * (float)GetFourByteSignedInt();
		v_printf(2, "(%.3f,%.3f) ", X, Y);
		if(i==0){
			firstX = X;
			firstY = Y;
		}
		if(thislayer->Height && thislayer->Show && CurrentObject){
			CurrentObject->GetCurrentPolygon()->AddPoint(i, X, Y);
		}
	}
	v_printf(2, "\n");
	if(thislayer->Height && thislayer->Show && CurrentObject){
		CurrentObject->GetCurrentPolygon()->AddPoint(i, firstX, firstY);
		//CurrentObject->GetCurrentPolygon()->SetColour(thislayer->Red, thislayer->Green, thislayer->Blue, thislayer->Filter, thislayer->Metal);
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
	currentdatatype = -1;
	currentmag = 1.0;
	currentbgnextn = 0.0;
	currentendextn = 0.0;
}

void GDSParse::ParseXY()
{
	float X, Y;
	float firstX=0.0, firstY=0.0, secondX=0.0, secondY=0.0;
	struct ProcessLayer *thislayer;
	int Flipped;

	thislayer = process->GetLayer(currentlayer, currentdatatype);
	Flipped = ((unsigned short)(currentstrans & 0x8000) == (unsigned short)0x8000) ? 1 : 0;

	switch(currentelement){
		case elSRef:
			SRefElements++;
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f)\n", X, Y);

			if(CurrentObject){
				CurrentObject->AddSRef(sname, X, Y, Flipped, currentmag);
				if(currentangle){
					CurrentObject->SetSRefRotation(0, -currentangle, 0);
				}
			}
			break;

		case elARef:
			ARefElements++;
			firstX = units * (float)GetFourByteSignedInt();
			firstY = units * (float)GetFourByteSignedInt();
			secondX = units * (float)GetFourByteSignedInt();
			secondY = units * (float)GetFourByteSignedInt();
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f) ", firstX, firstY);
			v_printf(2, "(%.3f,%.3f) ", secondX, secondY);
			v_printf(2, "(%.3f,%.3f)\n", X, Y);

			if(CurrentObject){
				CurrentObject->AddARef(sname, firstX, firstY, secondX, secondY, X, Y, arraycols, arrayrows, Flipped, currentmag);
				if(currentangle){
					CurrentObject->SetARefRotation(0, -currentangle, 0);
				}
			}
			break;

		case elText:
			TextElements++;

			if(thislayer==NULL){
				v_printf(2, "Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
				v_printf(2, "\tIgnoring this string.\n");
				while(recordlen){
					GetFourByteSignedInt();
				}
				currentwidth = 0.0; // Always reset to default for paths in case width not specified
				currentpathtype = 0;
				currentangle = 0.0;
				currentdatatype = 0;
				currentmag = 1.0;
				return;
			}

			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			v_printf(2, "(%.3f,%.3f)\n", X, Y);

			if(CurrentObject){
				int vert_just, horiz_just;

				vert_just = (((((unsigned long)currentpresentation & 0x8 ) == (unsigned long)0x8 ) ? 2 : 0) + (((((unsigned long)currentpresentation & 0x4 ) == (unsigned long)0x4 ) ? 1 : 0)));
				horiz_just = (((((unsigned long)currentpresentation & 0x2 ) == (unsigned long)0x2 ) ? 2 : 0) + (((((unsigned long)currentpresentation & 0x1 ) == (unsigned long)0x1 ) ? 1 : 0)));

				CurrentObject->AddText(X, Y, units*thislayer->Height, Flipped, currentmag, vert_just, horiz_just, thislayer);
				if(currentangle){
					CurrentObject->GetCurrentText()->SetRotation(0.0, -currentangle, 0.0);
				}
			}
			break;
		default:
			while(recordlen){
				GetFourByteSignedInt();
			}
			break;
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
	currentdatatype = -1;
	currentmag = 1.0;
	currentpresentation = 0;
}

short GDSParse::GetBitArray()
{
	byte byte1;

	fread(&byte1, 1, 1, iptr);
	fread(&byte1, 1, 1, iptr);

	recordlen-=2;
	return 0;
}

double GDSParse::GetEightByteReal()
{
	byte value;
	byte b8, b2, b3, b4, b5, b6, b7;
	double sign=1.0;
	double exponent;
	double mant;

	fread(&value, 1, 1, iptr);
	if(value & 128){
		value -= 128;
		sign = -1.0;
	}
	exponent = (double )value;
	exponent -= 64.0;
	mant=0.0;

	fread(&b2, 1, 1, iptr);
	fread(&b3, 1, 1, iptr);
	fread(&b4, 1, 1, iptr);
	fread(&b5, 1, 1, iptr);
	fread(&b6, 1, 1, iptr);
	fread(&b7, 1, 1, iptr);
	fread(&b8, 1, 1, iptr);

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

	recordlen-=8;

	return sign*(mant*pow(16.0,exponent));
}

long GDSParse::GetFourByteSignedInt()
{
	long value;
	fread(&value, 4, 1, iptr);
	
	recordlen-=4;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	return endian_swap_long(value);
#else
	return value;
#endif
}

short GDSParse::GetTwoByteSignedInt()
{
	short value;

	fread(&value, 2, 1, iptr);

	recordlen-=2;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	return endian_swap_short(value);
#else
	return value;
#endif
}

char *GDSParse::GetAsciiString()
{
	char *str=NULL;
	
	if(recordlen>0){
		recordlen += recordlen%2; /* Make sure length is even */
		str = new char[recordlen+1];
		if(!str){
			fprintf(stderr, "Unable to allocate memory for ascii string (%d)\n", recordlen);
			fclose(iptr);
			// FIXME - need to cleanup properly.
			exit(1);
		}
		fread(str, 1, recordlen, iptr);
		str[recordlen] = 0;
		recordlen = 0;
	}
	return str;
}

void GDSParse::ReportUnsupported(char *Name, enum RecordNumbers rn)
{
	if(!unsupported[rn]){
		v_printf(1, "Unsupported GDS2 record type: %s\n", Name);
		unsupported[rn] = true;
	}

}

