#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdsparse.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"

GDSParse::GDSParse (char *infile, char *outfile, char *processfile)
{
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
	currentdatatype = 0;

	mirror=0;

	process = new GDSProcess(processfile);
	if(process->LayerCount()==0){
		printf("Error: No layers found in \"%s\".\n", processfile);
		return;
	}else if(!process->IsValid()){
		printf("Error: Invalid process file\n");
		return;
	}


	iptr = fopen(infile, "rb");
	if(iptr){
		optr = fopen(outfile, "wt");
		if(optr){
			Objects = new GDSObjects;
			Parse();

			OutputPOVHeader();

			/* FIXME - need a better solution than this.
			**			all this does is to output the objects
			**			that have no SREFs or AREFs first and
			**			hope that the rest of the hierarchy works
			**			out ok.
			**		It is also inefficient with multiple calls to
			**			GetObject.
			*/

			int i;
			for(i=Objects->GetCount()-1; i>=0; i--){
				if(!Objects->GetObject(i)->HasASRef()){
					Objects->GetObject(i)->OutputToFile(optr);
				}
			}
			for(i=Objects->GetCount()-1; i>=0; i--){
				if(Objects->GetObject(i)->HasASRef()){
					Objects->GetObject(i)->OutputToFile(optr);
				}
			}
			fprintf(optr, "object { str_%s }\n", Objects->GetObject(0)->GetName());

			fclose(optr);
			optr = NULL;
		}
		fclose(iptr);
		iptr = NULL;

		printf("\nSummary:\n\tPaths:\t\t%ld\n\tBoundaries:\t%ld\n\tStrings:\t%ld\n\tStuctures:\t%ld\n\tArrays:\t\t%ld\n",
			PathElements, BoundaryElements, TextElements, SRefElements, ARefElements);
	}else{
		printf("Unable to read %s\n", infile);
	}
}

GDSParse::~GDSParse ()
{
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

		CameraPosition cp;
		cp = cpCentre;

		float modifier = (float)1.0;

		switch(cp){
			case cpCentre:
				// Default camera angle = 67.38
				// Half of this is 33.69
				// tan(33.69) = 0.66666 = 1/1.5
				// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8

				fprintf(optr, "camera {\n\tlocation <%.2f,%.2f,%.2f>\n", centreX, distance, centreY);
				fprintf(optr, "\tlook_at <%.2f,0,%.2f>\n}", centreX, centreY);
				break;
			case cpTopLeft:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*modifier, distance*0.4, Boundary->YMax*modifier);
				fprintf(optr, "\tlook_at <%.2f,0, %.2f>\n}", Boundary->XMax/modifier, Boundary->YMin/modifier);
				fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", Boundary->XMax*2, distance*2, Boundary->YMin*2);
				break;
			case cpTopRight:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*modifier, distance*0.4, Boundary->YMax*modifier);
				fprintf(optr, "\tlook_at <%.2f,0,%.2f>\n}", Boundary->XMin/modifier, Boundary->YMin/modifier);
				fprintf(optr, "light_source { <%.2f,%.2f,%.2f> White }\n", Boundary->XMin*2, distance*2, Boundary->YMin*2);
				break;
			case cpBottomLeft:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*modifier, distance*0.4, Boundary->YMin*modifier);
				fprintf(optr, "\tlook_at <%.2f,0,%.2f>\n}", Boundary->XMax/modifier, Boundary->YMax/modifier);
				fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", Boundary->XMax*2, distance*2, Boundary->YMax*2);
				break;
			case cpBottomRight:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*modifier, distance*0.4, Boundary->YMin*modifier);
				fprintf(optr, "\tlook_at <%.2f,0,%.2f>\n}", Boundary->XMin/modifier, Boundary->YMax/modifier);
				fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", Boundary->XMin*2, distance*2, Boundary->YMax*2);
				break;
		}

		fprintf(optr, "background { color Black }\n");
		fprintf(optr, "global_settings { ambient_light rgb <1.2,1.2,1.2> }\n");

		// Place camera far above in centre
		fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", centreX, distance*20, centreY);
//		fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", Boundary->XMin*2, distance*2, Boundary->YMax*2);
//		fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", Boundary->XMax, distance*2, Boundary->YMin);
//		fprintf(optr, "light_source { <%.2f, %.2f, %.2f> White }\n", Boundary->XMin, distance*2, Boundary->YMin);
//		fprintf(optr, "light_source { <2000, 2000, -30000> White }\n");
//		fprintf(optr, "light_source { <10000, 20000, -10000> White }\n");
//		fprintf(optr, "light_source { <30000, 20000, 10000> White }\n");
	}
}

void GDSParse::Parse()
{
	byte recordtype, datatype;

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
				ParseHeader();
				break;
			case rnBgnLib:
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnLibName:
				ParseLibName();
				break;
			case rnUnits:
				ParseUnits();
				break;
			case rnEndLib:
			case rnEndStr:
			case rnEndEl:
				/* Empty, no need to parse */
				break;
			case rnBgnStr:
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnStrName:
				ParseStrName();
				break;
			case rnBoundary:
				currentelement = elBoundary;
				break;
			case rnPath:
				currentelement = elPath;
				break;
			case rnSRef:
				currentelement = elSRef;
				break;
			case rnARef:
				currentelement = elARef;
				break;
			case rnText:
				currentelement = elText;
				break;
			case rnLayer:
				currentlayer = GetTwoByteSignedInt();
				break;
			case rnDataType:
				currentdatatype = GetTwoByteSignedInt();
				break;
			case rnWidth:
				currentwidth = (float)(GetFourByteSignedInt()/2);
				if(currentwidth > 0){
					currentwidth *= units;
				}
				// Scale to a half to make width correct when adding and
				// subtracting
				break;
			case rnXY:
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
				break;
			case rnSName:
				ParseSName();
				break;
			case rnPathType:
				//printf("PATHTYPE\n");
				currentpathtype = GetTwoByteSignedInt();
				break;
			case rnTextType:
				printf("TEXTTYPE\n");
				currenttexttype = GetTwoByteSignedInt();
				break;
			case rnPresentation:
				printf("PRESENTATION\n");
				currentpresentation = GetTwoByteSignedInt();
				break;
			case rnString:
				if(textstring){
					delete textstring;
					textstring = NULL;
				}
				textstring = GetAsciiString();
				if(CurrentObject && textstring){
					CurrentObject->SetTextString(textstring);
					delete textstring;
					textstring = NULL;
				}
				break;
			case rnSTrans:
				//FIXME
				/*currentpresentation = */
				currentstrans = GetTwoByteSignedInt();
				break;
			case rnMag:
				printf("MAG\n");
				//FIXME
				/*currentpresentation = */
				while(recordlen){
					GetEightByteReal();
				}
				break;
			case rnAngle:
				currentangle = (float)GetEightByteReal();
				//FIXME
				break;
/*			case rnUInteger:
				break;
Not Used	case rnUString:
				break;
*/
			case rnRefLibs:
				printf("REFLIBS\n");
				delete GetAsciiString();
				break;
			case rnFonts:
				printf("FONTS\n");
				delete GetAsciiString();
				break;
			case rnGenerations:
				printf("GENERATIONS\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnAttrTable:
				printf("ATTRTABLE\n");
				delete GetAsciiString();
				break;
			case rnStypTable:
				printf("STYPTABLE\n");
				delete GetAsciiString();
				break;
			case rnStrType:
				printf("STRTYPE\n");
				delete GetAsciiString();
				break;
			case rnElFlags:
				printf("ELFLAGS\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnElKey:
				printf("ELKEY\n");
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnLinkType:
				printf("LINKTYPE\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnLinkKeys:
				printf("LINKKEYS\n");
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnNodeType:
				printf("NODETYPE\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnPropAttr:
				printf("PROPATTR\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnPropValue:
				printf("PROPVALUE\n");
				delete GetAsciiString();
				break;
			case rnBox:
				printf("BOX\n");
				/* Empty */
				break;
			case rnBoxType:
				printf("BOXTYPE\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnPlex:
				printf("PLEX\n");
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnBgnExtn:
				printf("BGNEXTN\n");
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnEndExtn:
				printf("ENDEXTN\n");
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnTapeNum:
				printf("TAPENUM\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnTapeCode:
				printf("TAPECODE\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnStrClass:
				printf("STRCLASS\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnReserved:
				printf("RESERVED\n");
				/* Empty */
				break;
			case rnFormat:
				printf("FORMAT\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnMask:
				printf("MASK\n");
				delete GetAsciiString();
				break;
			case rnEndMasks:
				printf("ENDMASKS\n");
				/* Empty */
				break;
			case rnLibDirSize:
				printf("LIBDIRSIZE\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnSrfName:
				printf("SRFNAME\n");
				delete GetAsciiString();
				break;
			case rnLibSecur:
				printf("LIBSECUR\n");
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnBorder:
				printf("BORDER\n");
				/* Empty */
				break;
			case rnSoftFence:
				printf("SOFTFENCE\n");
				/* Empty */
				break;
			case rnHardFence:
				printf("HARDFENCE\n");
				/* Empty */
				break;
			case rnSoftWire:
				printf("SOFTWIRE\n");
				/* Empty */
				break;
			case rnHardWire:
				printf("HARDWIRE\n");
				/* Empty */
				break;
			case rnPathPort:
				printf("PATHPORT\n");
				/* Empty */
				break;
			case rnNodePort:
				printf("NODEPORT\n");
				/* Empty */
				break;
			case rnUserConstraint:
				printf("USERCONSTRAINT\n");
				/* Empty */
				break;
			case rnSpacerError:
				printf("SPACERERROR\n");
				/* Empty */
				break;
			case rnContact:
				printf("CONTACT\n");
				/* Empty */
				break;
			default:
				printf("Unknown record type (%d) at position %ld.", recordtype, ftell(iptr));

				return;
				break;
	
		}
	}	
}

void GDSParse::ParseHeader()
{
	short version;
	version = GetTwoByteSignedInt();
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
	}else{
		printf("Unable to allocate memory for string (%d)\n", strlen(str)+1);
	}
	delete str;
}

void GDSParse::ParseSName()
{
	char *str;
	str = GetAsciiString();
	if(sname){
		delete sname;
		sname = NULL;
	}
	sname = new char[strlen(str)+1];
	if(sname){
		strcpy(sname, str);
	}else{
		printf("Unable to allocate memory for string (%d)\n", strlen(str)+1);
	}
	delete str;
}

void GDSParse::ParseUnits()
{
	double tmp;
	units = (float)GetEightByteReal();
	tmp = GetEightByteReal();
	printf("db units/user units = %g\nsize of db units in metres = %g\nsize of user units in m = %g\n",units, tmp, tmp/units);
}

void GDSParse::ParseStrName()
{
	char *str;
	str = GetAsciiString();
	
	CurrentObject = Objects->AddObject(str);
	delete str;
}

void GDSParse::ParseXYPath()
{
	float X, Y;
	int points = recordlen/8;
	int i;
	struct ProcessLayer *thislayer;

	thislayer = process->GetLayer(currentlayer, currentdatatype);

	if(thislayer==NULL){
		printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
		printf("\tIgnoring this path\n.");
		while(recordlen){
			GetFourByteSignedInt();
		}
		currentwidth = 0.0; // Always reset to default for paths in case width not specified
		currentpathtype = 0;
		currentangle = 0.0;
		currentdatatype = 0;
		return;
	}

	if(currentwidth){
		/* FIXME - need to check for -ve value and then not scale */
		if(thislayer->Height && thislayer->Show && CurrentObject){
			CurrentObject->AddPath(units*thislayer->Height, units*thislayer->Thickness, points, currentwidth);
			CurrentObject->SetPathColour(thislayer->Red, thislayer->Green, thislayer->Blue, thislayer->Filter, thislayer->Metal);
		}
		for(i=0; i<points; i++){
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			if(thislayer->Height && thislayer->Show && CurrentObject){
				CurrentObject->AddPathPoint(i, X, Y);
			}
		}
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
	currentdatatype = 0;
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
		printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
		printf("\tIgnoring this boundary\n.");
		while(recordlen){
			GetFourByteSignedInt();
		}
		currentwidth = 0.0; // Always reset to default for paths in case width not specified
		currentpathtype = 0;
		currentangle = 0.0;
		currentdatatype = 0;
		return;
	}

	if(thislayer->Height && thislayer->Show && CurrentObject){
		CurrentObject->AddPrism(units*thislayer->Height, units*thislayer->Thickness, points+1);
	}

	for(i=0; i<points; i++){
		X = units * (float)GetFourByteSignedInt();
		Y = units * (float)GetFourByteSignedInt();
		if(i==0){
			firstX = X;
			firstY = Y;
		}
		if(thislayer->Height && thislayer->Show && CurrentObject){
			CurrentObject->AddPrismPoint(i, X, Y);
		}
	}
	if(thislayer->Height && thislayer->Show && CurrentObject){
		CurrentObject->AddPrismPoint(i, firstX, firstY);
		CurrentObject->SetPrismColour(thislayer->Red, thislayer->Green, thislayer->Blue, thislayer->Filter, thislayer->Metal);
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
	currentdatatype = 0;
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

			if(CurrentObject){
				CurrentObject->AddSRef(sname, X, Y, Flipped);
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

			if(CurrentObject){
				CurrentObject->AddARef(sname, firstX, firstY, secondX, secondY, X, Y, arraycols, arrayrows, Flipped);
				if(currentangle){
					CurrentObject->SetARefRotation(0, -currentangle, 0);
				}
			}


			break;
		case elText:
			TextElements++;

			if(thislayer==NULL){
				printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
				printf("\tIgnoring this string\n.");
				while(recordlen){
					GetFourByteSignedInt();
				}
				currentwidth = 0.0; // Always reset to default for paths in case width not specified
				currentpathtype = 0;
				currentangle = 0.0;
				currentdatatype = 0;
				return;
			}


			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();

			if(CurrentObject){
				CurrentObject->AddText(X, Y, units*thislayer->Height, Flipped);
				CurrentObject->SetTextColour(thislayer->Red, thislayer->Green, thislayer->Blue, thislayer->Filter, thislayer->Metal);
				if(currentangle){
					CurrentObject->SetTextRotation(0.0, -currentangle, 0.0);
				}
			}
			break;
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
	currentdatatype = 0;
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

	return endian_swap_long(value);
}

short GDSParse::GetTwoByteSignedInt()
{
	short value;

	fread(&value, 2, 1, iptr);

	recordlen-=2;

	return endian_swap_short(value);
}

char *GDSParse::GetAsciiString()
{
	char *str=NULL;
	
	if(recordlen>0){
		recordlen += recordlen%2; /* Make sure length is even */
		str = new char[recordlen+1];
		if(!str){
			printf("Unable to allocate memory for ascii string (%d)\n", recordlen);
			fclose(iptr);
			// FIXME - need to cleanup properly.
			exit(1);
		}
		fread(str, 1, recordlen, iptr);
		str[recordlen] = 0;
		recordlen = 0;
	}
	if(!str){
		printf("eek!\n");
	}
	return str;
}
