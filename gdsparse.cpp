#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdsparse.h"
#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"

GDSParse::GDSParse (char *infile, char *outfile, char *configfile, char *processfile)
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
	currentmag = 1.0;

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
		optr = fopen(outfile, "wt");
		if(optr){
			Objects = new GDSObjects;
			Parse();

			OutputPOVHeader();

			RecursiveOutput(Objects->GetObject(0), optr);
			fprintf(optr, "object { str_%s }\n", Objects->GetObject(0)->GetName());

			fclose(optr);
			optr = NULL;
		}
		fclose(iptr);
		iptr = NULL;

		printf("\nSummary:\n\tPaths:\t\t%ld\n\tBoundaries:\t%ld\n\tStrings:\t%ld\n\tStuctures:\t%ld\n\tArrays:\t\t%ld\n",
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

void GDSParse::RecursiveOutput(class GDSObject *Object, FILE *optr)
{
	if(Object->HasASRef()){
		GDSObject *child;

		int i=0;
		do{
			child=Objects->GetObject(Object->GetSRefName(i));
			i++;
			if(child){
				RecursiveOutput(child, optr);
			}
		}while(child);

		i = 0;
		do{
			child=Objects->GetObject(Object->GetARefName(i));
			i++;
			if(child){
				RecursiveOutput(child, optr);
			}
		}while(child);

	}
	Object->OutputToFile(optr, Objects);
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
				ReportUnsupported("PATHTYPE", rnPathType);
				currentpathtype = GetTwoByteSignedInt();
				break;
			case rnTextType:
				ReportUnsupported("TEXTTYPE", rnTextType);
				currenttexttype = GetTwoByteSignedInt();
				break;
			case rnPresentation:
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
				if(!unsupported[rnSTrans]){
					printf("Incomplete support for GDS2 record type: STRANS\n");
					unsupported[rnSTrans] = true;
				}
				//FIXME
				currentstrans = GetTwoByteSignedInt();
				break;
			case rnMag:
				currentmag = GetEightByteReal();
				break;
			case rnAngle:
				currentangle = (float)GetEightByteReal();
				break;
/*			case rnUInteger:
				break;
Not Used	case rnUString:
				break;
*/
			case rnRefLibs:
				ReportUnsupported("REFLIBS", rnRefLibs);
				delete GetAsciiString();
				break;
			case rnFonts:
				ReportUnsupported("FONTS", rnFonts);
				delete GetAsciiString();
				break;
			case rnGenerations:
				ReportUnsupported("GENERATIONS", rnGenerations);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnAttrTable:
				ReportUnsupported("ATTRTABLE", rnAttrTable);
				delete GetAsciiString();
				break;
			case rnStypTable:
				ReportUnsupported("STYPTABLE", rnStypTable);
				delete GetAsciiString();
				break;
			case rnStrType:
				ReportUnsupported("STRTYPE", rnStrType);
				delete GetAsciiString();
				break;
			case rnElFlags:
				ReportUnsupported("ELFLAGS", rnElFlags);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnElKey:
				ReportUnsupported("ELKEY", rnElKey);
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnLinkType:
				ReportUnsupported("LINKTYPE", rnLinkType);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnLinkKeys:
				ReportUnsupported("LINKKEYS", rnLinkKeys);
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnNodeType:
				ReportUnsupported("NODETYPE", rnNodeType);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnPropAttr:
				ReportUnsupported("PROPATTR", rnPropAttr);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnPropValue:
				ReportUnsupported("PROPVALUE", rnPropValue);
				delete GetAsciiString();
				break;
			case rnBox:
				ReportUnsupported("BOX", rnBox);
				/* Empty */
				break;
			case rnBoxType:
				ReportUnsupported("BOXTYPE", rnBoxType);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnPlex:
				ReportUnsupported("PLEX", rnPlex);
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnBgnExtn:
				ReportUnsupported("BGNEXTN", rnBgnExtn);
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnEndExtn:
				ReportUnsupported("ENDEXTN", rnEndExtn);
				while(recordlen){
					GetFourByteSignedInt();
				}
				break;
			case rnTapeNum:
				ReportUnsupported("TAPENUM", rnTapeNum);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnTapeCode:
				ReportUnsupported("TAPECODE", rnTapeCode);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnStrClass:
				ReportUnsupported("STRCLASS", rnStrClass);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnReserved:
				ReportUnsupported("RESERVED", rnReserved);
				/* Empty */
				break;
			case rnFormat:
				ReportUnsupported("FORMAT", rnFormat);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnMask:
				ReportUnsupported("MASK", rnMask);
				delete GetAsciiString();
				break;
			case rnEndMasks:
				ReportUnsupported("ENDMASKS", rnEndMasks);
				/* Empty */
				break;
			case rnLibDirSize:
				ReportUnsupported("LIBDIRSIZE", rnLibDirSize);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnSrfName:
				ReportUnsupported("SRFNAME", rnSrfName);
				delete GetAsciiString();
				break;
			case rnLibSecur:
				ReportUnsupported("LIBSECUR", rnLibSecur);
				while(recordlen){
					GetTwoByteSignedInt();
				}
				break;
			case rnBorder:
				ReportUnsupported("BORDER", rnBorder);
				/* Empty */
				break;
			case rnSoftFence:
				ReportUnsupported("SOFTFENCE", rnSoftFence);
				/* Empty */
				break;
			case rnHardFence:
				ReportUnsupported("HARDFENCE", rnHardFence);
				/* Empty */
				break;
			case rnSoftWire:
				ReportUnsupported("SOFTWIRE", rnSoftWire);
				/* Empty */
				break;
			case rnHardWire:
				ReportUnsupported("HARDWIRE", rnHardWire);
				/* Empty */
				break;
			case rnPathPort:
				ReportUnsupported("PATHPORT", rnPathPort);
				/* Empty */
				break;
			case rnNodePort:
				ReportUnsupported("NODEPORT", rnNodePort);
				/* Empty */
				break;
			case rnUserConstraint:
				ReportUnsupported("USERCONSTRAINT", rnUserConstraint);
				/* Empty */
				break;
			case rnSpacerError:
				ReportUnsupported("SPACERERROR", rnSpacerError);
				/* Empty */
				break;
			case rnContact:
				ReportUnsupported("CONTACT", rnContact);
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
		fprintf(stderr, "Unable to allocate memory for string (%d)\n", strlen(str)+1);
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
		for(int i=0; i<strlen(sname); i++){
			if(sname[i] == '$'){
				sname[i] = '_';
			}
		}
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
	printf("DB units/user units = %g\nSize of DB units in metres = %g\nSize of user units in m = %g\n\n", 1/units, tmp, tmp/units);
}

void GDSParse::ParseStrName()
{
	char *str=NULL;
	str = GetAsciiString();

	if(str){
		for(int i=0; i<strlen(str); i++){
			if(str[i] == '$'){
				str[i] = '_';
			}
		}

		CurrentObject = Objects->AddObject(str);
		delete str;
	}
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
		printf("\tIgnoring this path.\n");
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
	currentmag = 1.0;
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
		printf("\tIgnoring this boundary.\n");
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
	currentmag = 1.0;
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
				printf("Notice: Layer found in gds2 file that is not defined in the process configuration. Layer is %d, datatype %d.\n", currentlayer, currentdatatype);
				printf("\tIgnoring this string.\n");
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

			if(CurrentObject){
				int vert_just, horiz_just;

				vert_just = (((((unsigned long)currentpresentation & 0x8 ) == (unsigned long)0x8 ) ? 2 : 0) + (((((unsigned long)currentpresentation & 0x4 ) == (unsigned long)0x4 ) ? 1 : 0)));
				horiz_just = (((((unsigned long)currentpresentation & 0x2 ) == (unsigned long)0x2 ) ? 2 : 0) + (((((unsigned long)currentpresentation & 0x1 ) == (unsigned long)0x1 ) ? 1 : 0)));

				CurrentObject->AddText(X, Y, units*thislayer->Height, Flipped, currentmag, vert_just, horiz_just);
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
		printf("Unsupported GDS2 record type: %s\n", Name);
		unsupported[rn] = true;
	}

}
