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

	PathElements = 0;
	BoundaryElements = 0;
	TextElements = 0;
	SRefElements = 0;
	ARefElements = 0;

	mirror=0;

	process = new GDSProcess(processfile);
	if(process->LayerCount()==0){
		printf("No layers found in \"%s\".\n", processfile);
		return;
	}


	iptr = fopen(infile, "rb");
	if(iptr){
		optr = fopen(outfile, "wt");
		if(optr){
			Objects = new GDSObjects;
			Parse();

			OutputPOVHeader();
			for(int i=Objects->GetCount()-1; i>=0; i--){
				Objects->GetObject(i)->OutputToFile(optr);
			}
			fprintf(optr, "object { str_%s }\n", Objects->GetObject(0)->GetName());

			fclose(optr);
			optr = NULL;
		}
		fclose(iptr);
		iptr = NULL;
	}else{
		printf("Unable to read %s\n", infile);
	}
	printf("Summary:\n\tPaths:\t\t%ld\n\tBoundaries:\t%ld\n\tStrings:\t%ld\n\tStuctures:\t%ld\n\tArrays:\t\t%ld\n",
		PathElements, BoundaryElements, TextElements, SRefElements, ARefElements);
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
		cp = cpTopRight;

		float modifier = (float)1.2;

		switch(cp){
			case cpCentre:
				// Default camera angle = 67.38
				// Half of this is 33.69
				// tan(33.69) = 0.66666 = 1/1.5
				// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8

				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", centreX, distance, centreY);
				fprintf(optr, "\tlook_at <%.2f, 2000, %.2f>\n}", centreX, centreY);
				break;
			case cpTopLeft:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*modifier, distance*0.4, Boundary->YMax*modifier);
				fprintf(optr, "\tlook_at <%.2f, 2000, %.2f>\n}", Boundary->XMax/modifier, Boundary->YMin/modifier);
				break;
			case cpTopRight:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*modifier, distance*0.4, Boundary->YMax*modifier);
				fprintf(optr, "\tlook_at <%.2f, 2000, %.2f>\n}", Boundary->XMin/modifier, Boundary->YMin/modifier);
				break;
			case cpBottomLeft:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*modifier, distance*0.4, Boundary->YMin*modifier);
				fprintf(optr, "\tlook_at <%.2f, 2000, %.2f>\n}", Boundary->XMax/modifier, Boundary->YMax/modifier);
				break;
			case cpBottomRight:
				fprintf(optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*modifier, distance*0.4, Boundary->YMin*modifier);
				fprintf(optr, "\tlook_at <%.2f, 2000, %.2f>\n}", Boundary->XMin/modifier, Boundary->YMax/modifier);
				break;
		}

		fprintf(optr, "background { color Black }\n");
		fprintf(optr, "light_source { <2000, 2000, -30000> White }\n");
		fprintf(optr, "light_source { <10000, 20000, -10000> White }\n");
		fprintf(optr, "light_source { <30000, 20000, 10000> White }\n");
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
			case rnEndStr:
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
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnWidth:
				currentwidth = (float)(GetFourByteSignedInt()/2);
//				if(currentwidth > 0){
///					currentwidth *= units;
//				}
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
			case rnEndEl:
				/* Empty, no need to parse */
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
			case rnAttrTable:
				printf("ATTRTABLE\n");
				break;
			case rnStypTable:
				printf("STYPTABLE\n");
				break;
			case rnStrType:
				printf("STRTYPE\n");
				break;
			case rnElFlags:
				printf("ELFLAGS\n");
				break;
			case rnElKey:
				printf("ELKEY\n");
				break;
			case rnLinkType:
				printf("LINKTYPE\n");
				break;
			case rnLinkKeys:
				printf("LINKKEYS\n");
				break;
			case rnNodeType:
				printf("NODETYPE\n");
				break;
			case rnPropAttr:
				printf("PROPATTR\n");
				break;
			case rnPropValue:
				printf("PROPVALUE\n");
				break;
			case rnBox:
				printf("BOX\n");
				break;
			case rnBoxType:
				printf("BOXTYPE\n");
				break;
			case rnPlex:
				printf("PLEX\n");
				break;
			case rnBgnExtn:
				printf("BGNEXTN\n");
				break;
			case rnEndExtn:
				printf("ENDEXTN\n");
				break;
			case rnTapeNum:
				printf("TAPENUM\n");
				break;
			case rnTapeCode:
				printf("TAPECODE\n");
				break;
			case rnStrClass:
				printf("STRCLASS\n");
				break;
			case rnReserved:
				printf("RESERVED\n");
				break;
			case rnFormat:
				printf("FORMAT\n");
				break;
			case rnMask:
				printf("MASK\n");
				break;
			case rnEndMasks:
				printf("ENDMASKS\n");
				break;
			case rnLibDirSize:
				printf("LIBDIRSIZE\n");
				break;
			case rnSrfName:
				printf("SRFNAME\n");
				break;
			case rnLibSecur:
				printf("LIBSECUR\n");
				break;
			case rnBorder:
				printf("BORDER\n");
				break;
			case rnSoftFence:
				printf("SOFTFENCE\n");
				break;
			case rnHardFence:
				printf("HARDFENCE\n");
				break;
			case rnSoftWire:
				printf("SOFTWIRE\n");
				break;
			case rnHardWire:
				printf("HARDWIRE\n");
				break;
			case rnPathPort:
				printf("PATHPORT\n");
				break;
			case rnNodePort:
				printf("NODEPORT\n");
				break;
			case rnUserConstraint:
				printf("USERCONSTRAINT\n");
				break;
			case rnSpacerError:
				printf("SPACERERROR\n");
				break;
			case rnContact:
				printf("CONTACT\n");
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
	//FIXME - must scale things as floats!
	double tmp;
	units = (float)GetEightByteReal();
	tmp = GetEightByteReal();

	units = 1; // ignore scaling for the mo
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

	thislayer = process->GetLayer(currentlayer);

	if(thislayer==NULL){
		printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d.\n", currentlayer);
		return;
	}

	if(currentwidth){
		/* FIXME - need to check for -ve value and then not scale */
		if(thislayer->Height && thislayer->Show && CurrentObject){
			CurrentObject->AddPath(thislayer->Height, thislayer->Thickness, points, currentwidth);
		}
		for(i=0; i<points; i++){
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			if(thislayer->Height && thislayer->Show && CurrentObject){
				CurrentObject->AddPathPoint(i, X, Y);
			}
		}
	}
	if(thislayer->Height && thislayer->Show && CurrentObject){
		CurrentObject->SetPathColour(thislayer->Red, thislayer->Green, thislayer->Blue, thislayer->Filter, thislayer->Metal);
	}
	currentwidth = 0.0; // Always reset to default for paths in case width not specified
	currentpathtype = 0;
	currentangle = 0.0;
}

void GDSParse::ParseXYBoundary()
{
	float X, Y;
	float firstX=0.0, firstY=0.0;
	int points = recordlen/8;
	int i;
	struct ProcessLayer *thislayer;

	thislayer = process->GetLayer(currentlayer);

	if(thislayer==NULL){
		printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d.\n", currentlayer);
		return;
	}

	if(thislayer->Height && thislayer->Show && CurrentObject){
		CurrentObject->AddPrism(thislayer->Height, thislayer->Thickness, points+1);
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
}

void GDSParse::ParseXY()
{
	float X, Y;
	float firstX=0.0, firstY=0.0, secondX=0.0, secondY=0.0;
	struct ProcessLayer *thislayer;
	int Flipped;

	thislayer = process->GetLayer(currentlayer);

	if(thislayer==NULL){
		printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d.\n", currentlayer);
		return;
	}

	switch(currentelement){
		case elSRef:
			SRefElements++;
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();
			Flipped = ((unsigned short)(currentstrans & 0x8000) == (unsigned short)0x8000) ? 1 : 0;

			if(CurrentObject){
				CurrentObject->AddSRef(sname, X, Y, Flipped);
				if(currentangle){
					CurrentObject->SetSRefRotation(0, -currentangle, 0);
				}
			}
			break;
		case elARef:
			ARefElements++;
			//FIXME
			firstX = units * (float)GetFourByteSignedInt();
			firstY = units * (float)GetFourByteSignedInt();
			secondX = units * (float)GetFourByteSignedInt();
			secondY = units * (float)GetFourByteSignedInt();
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();

			Flipped = ((unsigned short)(currentstrans & 0x8000) == (unsigned short)0x8000) ? 1 : 0;

			CurrentObject->AddARef(sname, firstX, firstY, secondX, secondY, X, Y, arraycols, arrayrows, Flipped);
			CurrentObject->SetARefRotation(0, -currentangle, 0);


			break;
		case elText:
			TextElements++;
			X = units * (float)GetFourByteSignedInt();
			Y = units * (float)GetFourByteSignedInt();

			if(CurrentObject){
				CurrentObject->AddText(X, Y, thislayer->Height);
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
