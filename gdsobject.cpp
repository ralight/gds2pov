#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "gdsparse.h"
#include "process_cfg.h"

GDSParse::GDSParse (char *filename, csRef<iEngine> engine, iSector* room)
{
	GDSParse::engine = engine;
	GDSParse::room = room;

	csRef<iMeshWrapper> objs;
	csRef<iThingState> objstate;
	csRef<iMeshWrapper> refobj;

	infile = NULL;
	libname = NULL;
	sname = NULL;
	CurrentStrName = NULL;

	fudge = 0;
	StrNames = NULL;

	CurrentStr=0;
	StrCount = 0;
	mirror=0;

	infile = fopen(filename, "rb");
	if(infile){
		FindStructures();
		for(CurrentStr=StrCount-1; CurrentStr>=0; CurrentStr--){
			objs = engine->CreateThingMesh (room, StrNames[CurrentStr]);
			objstate = SCF_QUERY_INTERFACE (objs->GetMeshObject (), iThingState);
			walls_state = objstate->GetFactory ();

			Parse();
		}
		for(CurrentStr=1; CurrentStr<StrCount; CurrentStr++){
				refobj = engine->FindMeshObject(StrNames[CurrentStr]);
				engine->RemoveObject(refobj);
		}
		fclose(infile);
		infile = NULL;
	}else{
		printf("Unable to read %s\n", filename);
	}
}

GDSParse::~GDSParse ()
{
	for(int i=0; i<StrCount; i++){
		delete StrNames[i];
	}
	free(StrNames);
	if(infile){
		fclose(infile);
		infile = NULL;
	}
	if(sname){
		delete sname;
	}
	if(CurrentStrName){
		delete CurrentStrName;
	}
}

void GDSParse::FindStructures()
{
	StrCount = 0;
	FindingStr = 1;
	Parse();
	StrNames = (char **)calloc(StrCount, sizeof(char*));
	FindingStr = 2;
	Parse();
#ifdef DEBUG
	for(int i=0; i<StrCount; i++){
		printf("%d %s\n", i, StrNames[i]);
	}
#endif
	FindingStr = 0;
}

void GDSParse::Parse()
{
	fseek(infile, 0, SEEK_SET);
	if(walls_state){
		walls_state->RemovePolygons();
	}

#ifdef DEBUG
	if(FindingStr==0){
		printf("\nStarting %s\n", StrNames[CurrentStr]);
	}
#endif
	while(!feof(infile)){
		recordlen = GetTwoByteSignedInt();
		fread(&recordtype, 1, 1, infile);
		fread(&datatype, 1, 1, infile);
		recordlen -= 4;
		switch(recordtype){
			case rnHeader:
				ParseHeader();
				break;
			case rnBgnLib:
				ParseBgnLib();
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
				ParseBgnStr();
				break;
			case rnStrName:
				ParseStrName();
				break;
			case rnEndStr:
				/* Empty, no need to parse */
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
				/* Empty, no need to parse */
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
				if(currentwidth > 0){
					currentwidth *= units;
				}
				// Scale to a half to make width correct when adding and
				// subtracting
				break;
			case rnXY:
				ParseXY();
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
				currentpathtype = GetTwoByteSignedInt();
				break;
			case rnTextType:
				currenttexttype = GetTwoByteSignedInt();
				break;
			case rnPresentation:
				currentpresentation = GetTwoByteSignedInt();
				break;
			case rnString:
				printf("STRING\n");
				//FIXME
				free(GetAsciiString());
				break;
			case rnSTrans:
				ParseSTrans();
				//FIXME
			case rnMag:
				printf("MAG\n");
				//FIXME
				/*currentpresentation = */
				while(recordlen){
					GetEightByteReal();
				}
				break;
			case rnAngle:
				angle = (float)GetEightByteReal();
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
				printf("Unknown record type (%d)", recordtype);
				return;
				break;
	
		}
	}
#ifdef DEBUG
	if(FindingStr==0){
		printf("%s has %d polygons\n", StrNames[CurrentStr], walls_state->GetPolygonCount());
	}
#endif
}

void GDSParse::ParseHeader()
{
	gdsversion = GetTwoByteSignedInt();
}

void GDSParse::ParseBgnLib()
{
	short tmp;
	while(recordlen){
		tmp = GetTwoByteSignedInt();
	}
}

void GDSParse::ParseLibName()
{
	char *str;
	str = GetAsciiString();
	if(str){
		free(str);
	}
}

void GDSParse::ParseSName()
{
	char *str;
	if(sname){
		delete sname;
		sname = NULL;
	}
	str = GetAsciiString();
	if(str){
		sname = new char[strlen(str)+1];
		strncpy(sname, str, strlen(str)+1);
		free(str);
	}
}

void GDSParse::ParseUnits()
{
	double tmp;
	units = (float)GetEightByteReal();
	tmp = GetEightByteReal();
}

void GDSParse::ParseBgnStr()
{
	short tmp;
	while(recordlen){
		tmp = GetTwoByteSignedInt();
	}
}

void GDSParse::ParseSTrans()
{
	unsigned short tmp;
	tmp = GetTwoByteSignedInt();
	if(tmp & 32768){
		mirror = 1;
	}
}

void GDSParse::ParseStrName()
{
	char *str;

	if(FindingStr == 1){
		StrCount++;
	}
	if(CurrentStrName){
		delete CurrentStrName;
		CurrentStrName = NULL;
	}
	str = GetAsciiString();
	if(str){
		CurrentStrName = new char[strlen(str)+1];
		strncpy(CurrentStrName, str, strlen(str)+1);
		if(FindingStr == 2){
			StrNames[CurrentStr] = new char[strlen(str)+1];
			strncpy(StrNames[CurrentStr],str, strlen(str)+1);
			CurrentStr++;
		}
		free(str);
	}
}

void GDSParse::ParseXY()
{
	layers thislayer;
	iMaterialWrapper* tm=NULL;

	layertable(currentlayer, &thislayer);

	if(thislayer.height && thislayer.show && (FindingStr==0) && (strncmp(StrNames[CurrentStr], CurrentStrName, strlen(CurrentStrName)+1)==0)){
		tm = engine->GetMaterialList ()->FindByName (thislayer.colour);
	}

	switch(currentelement){
		case elBoundary:
			HandleBoundary(tm, &thislayer);
			break;
		case elPath:
			HandlePath(tm, &thislayer);
			break;
		case elSRef:
			HandleSRef(tm);
			break;
		case elARef:
			HandleARef(tm);
			break;
	}
	currentwidth = 0.0; // Always reset to default in case not specified
	currentpathtype = 0;
	angle = 0.0;
	mirror = 0;
}

void GDSParse::HandleSRef(iMaterialWrapper* tm)
{
	float X, Y;
	int i, j;

	csRef<iMeshWrapper> refobj;
	csRef<iThingState> ref_thingstate;
	csRef<iThingFactoryState> ref_factorystate;
	csRef<iThingState> thingstate;
	csRef<iThingFactoryState> thingfact;
	csRef<iMeshObjectFactory> meshfact;
	csRef<iMeshObject> newobj;
	csRef<iMeshWrapper> newobjwrapper;
	csMatrix3 rot;
	csVector3 v;

	X = units * (float)GetFourByteSignedInt();
	Y = units * (float)GetFourByteSignedInt();

	if((FindingStr==0) && (strncmp(StrNames[CurrentStr], CurrentStrName, strlen(CurrentStrName)+1)==0)){
		refobj = engine->FindMeshObject(sname);
		ref_thingstate = SCF_QUERY_INTERFACE (refobj->GetMeshObject (), iThingState);
		ref_factorystate = ref_thingstate->GetFactory ();
		meshfact = SCF_QUERY_INTERFACE (ref_factorystate, iMeshObjectFactory);

		if(!mirror){
			newobj = meshfact->NewInstance();
			newobjwrapper = engine->CreateMeshWrapper(newobj, "newobj", room, csVector3(X,Y,0));

			rot = csZRotMatrix3 (-angle*3.141/180);
			csOrthoTransform ot (rot, newobjwrapper->GetMovable()->GetTransform().GetOrigin ());
			newobjwrapper->GetMovable()->SetTransform (ot);
			newobjwrapper->GetMovable()->UpdateMove();
		}else{
			//newobjwrapper->GetMovable()->
/*			rot = csYRotMatrix3 (-180*3.141/180);
			csOrthoTransform ot (rot, newobjwrapper->GetMovable()->GetTransform().GetOrigin ());
			newobjwrapper->GetMovable()->SetTransform (ot);
			newobjwrapper->GetMovable()->UpdateMove();

			thingstate = SCF_QUERY_INTERFACE (newobjwrapper->GetMeshObject (), iThingState);
			thingfact = thingstate->GetFactory();
			//for(i=0; i<thingfact->GetPolygonCount(); i++){
			//	for(j=0; j<thingfact->GetPolygonVertexCount(i); j++){
			for(i=0; i<thingfact->GetVertexCount(); i++){
					//v = thingfact->GetPolygonVertex(i, j);
					//v.z*=-1;
				v = thingfact->GetVertex(i);
				v.z *= -1;
				thingfact->SetVertex(i, v);
				//}
			}*/
		}
	}
}

void GDSParse::HandleARef(iMaterialWrapper* tm)
{
	float X1, Y1;
	float X2, Y2;
	float X3, Y3;
	float Xspacing, Yspacing;
	int i, j;

	csRef<iMeshWrapper> refobj;
	csRef<iThingState> ref_thingstate;
	csRef<iThingFactoryState> ref_factorystate;
	csRef<iMeshObjectFactory> meshfact;
	csRef<iMeshObject> newobj;
	csRef<iMeshWrapper> newobjwrapper;
	csMatrix3 rot;

	X1 = units * (float)GetFourByteSignedInt();
	Y1 = units * (float)GetFourByteSignedInt();
	X2 = units * (float)GetFourByteSignedInt();
	Y2 = units * (float)GetFourByteSignedInt();
	X3 = units * (float)GetFourByteSignedInt();
	Y3 = units * (float)GetFourByteSignedInt();

	Xspacing = (X2 - X1)/arraycols;
	Yspacing = (Y3 - Y1)/arrayrows;

	if((FindingStr==0) && (strncmp(StrNames[CurrentStr], CurrentStrName, strlen(CurrentStrName)+1)==0)){
		refobj = engine->FindMeshObject(sname);
		ref_thingstate = SCF_QUERY_INTERFACE (refobj->GetMeshObject (), iThingState);
		ref_factorystate = ref_thingstate->GetFactory ();
		meshfact = SCF_QUERY_INTERFACE (ref_factorystate, iMeshObjectFactory);

		for(i=0; i<arraycols; i++){
			for(j=0; j<arrayrows; j++){
				newobj = meshfact->NewInstance();
				newobjwrapper = engine->CreateMeshWrapper(newobj, "newobj_sref", room, csVector3(X1+(Xspacing*i),Y1+(Yspacing*j),0));

				rot = csZRotMatrix3 (-angle*3.141/180);
				csOrthoTransform ot (rot, newobjwrapper->GetMovable()->GetTransform().GetOrigin ());
				newobjwrapper->GetMovable()->SetTransform (ot);
				newobjwrapper->GetMovable()->UpdateMove();
			}
		}


/*		if(mirror){
			rot = csYRotMatrix3 (-180*3.141/180);
			csOrthoTransform ot (rot, newobjwrapper->GetMovable()->GetTransform().GetOrigin ());
			newobjwrapper->GetMovable()->SetTransform (ot);
			newobjwrapper->GetMovable()->UpdateMove();
		}*/
	}
}

void GDSParse::HandleBoundary(iMaterialWrapper* tm, layers *thislayer)
{
	float X, Y, nextX=0.0, nextY=0.0, prevX=0.0, prevY=0.0;
	float firstX=0.0, firstY=0.0, secondX=0.0, secondY=0.0;
	int points = recordlen/8;
	int i;//, j, k;
	csVector3 vi, vj, vk;
	//double angle_ij, angle_jk;

	int pindex=0;
	int topindex=0, botindex=0;
	int topindex2=0, botindex2=0;

	nextX = units * (float)GetFourByteSignedInt();
	nextY = units * (float)GetFourByteSignedInt();
	if(thislayer->height && thislayer->show && tm){
		topindex = walls_state->AddEmptyPolygon();
		walls_state->SetPolygonMaterial (csPolygonRange(topindex,topindex), tm);
		botindex = walls_state->AddEmptyPolygon();
		walls_state->SetPolygonMaterial (csPolygonRange(botindex,botindex), tm);
		walls_state->AddPolygonVertex(csPolygonRange(topindex,topindex),csVector3(nextX, nextY, units*(thislayer->height+thislayer->thickness)));
		walls_state->AddPolygonVertex(csPolygonRange(botindex,botindex),csVector3(nextX, nextY, units*thislayer->height));
	}
	while(recordlen){
		X = nextX;
		Y = nextY;
		nextX = units * (float)GetFourByteSignedInt();
		nextY = units * (float)GetFourByteSignedInt();

		if(thislayer->height && thislayer->show && tm){
			pindex = walls_state->AddQuad(	csVector3(X, Y, units*(thislayer->height+thislayer->thickness)), \
											csVector3(nextX, nextY, units*(thislayer->height+thislayer->thickness)), \
											csVector3(nextX, nextY, units*thislayer->height), \
											csVector3(X, Y, units*thislayer->height));
			walls_state->SetPolygonMaterial (csPolygonRange(pindex,pindex), tm);
			walls_state->SetPolygonTextureMapping (csPolygonRange(pindex,pindex), 3);
			pindex = walls_state->AddQuad(	csVector3(X, Y, units*thislayer->height), \
											csVector3(nextX, nextY, units*thislayer->height), \
											csVector3(nextX, nextY, units*(thislayer->height+thislayer->thickness)), \
											csVector3(X, Y, units*(thislayer->height+thislayer->thickness)));
			walls_state->SetPolygonMaterial (csPolygonRange(pindex,pindex), tm);
			walls_state->SetPolygonTextureMapping (csPolygonRange(pindex,pindex), 3);
			if(points==5){
				walls_state->AddPolygonVertex(csPolygonRange(topindex,topindex),csVector3(nextX, nextY, units*(thislayer->height+thislayer->thickness)));
				walls_state->AddPolygonVertex(csPolygonRange(botindex,botindex),csVector3(nextX, nextY, units*thislayer->height));
			}
		}
	}
	if(thislayer->height && thislayer->show && tm && points==5){
/*		topindex2 = walls_state->AddEmptyPolygon();
		walls_state->SetPolygonMaterial (csPolygonRange(topindex2,topindex2), tm);
		for(i=walls_state->GetPolygonVertexCount(topindex)-1; i>=0; i--){
			walls_state->AddPolygonVertex(csPolygonRange(topindex2,topindex2),walls_state->GetPolygonVertex(topindex,i));
		}*/
		botindex2 = walls_state->AddEmptyPolygon();
		for(i=walls_state->GetPolygonVertexCount(botindex)-1; i>=0; i--){
			walls_state->AddPolygonVertex(csPolygonRange(botindex2,botindex2),walls_state->GetPolygonVertex(botindex,i));
		}
		walls_state->SetPolygonMaterial (csPolygonRange(botindex2,botindex2), tm);
		walls_state->SetPolygonTextureMapping (csPolygonRange(botindex2,botindex2), 3);
		walls_state->SetPolygonTextureMapping (csPolygonRange(topindex,topindex), 3);
		walls_state->RemovePolygon(botindex);
/*
		for(i=0; i<walls_state->GetPolygonVertexCount(topindex); i++){
			vi = walls_state->GetPolygonVertex(topindex, i);
			for(j=0; j<walls_state->GetPolygonVertexCount(topindex)&&i!=j; j++){
				vj = walls_state->GetPolygonVertex(topindex, j);
				for(k=0; k<walls_state->GetPolygonVertexCount(topindex)&&j!=k; k++){
					vk = walls_state->GetPolygonVertex(topindex, k);
					angle_ij = atan2(vj.x-vi.x, vj.y-vi.y);
					angle_jk = atan2(vk.x-vj.x, vk.y-vj.y);
					if( (angle_jk - angle_ij) > 0 && (angle_jk - angle_ij) < (3.1415926/2)){
						//printf("ij=%f, jk=%f, ij-jk=%f\n", angle_ij, angle_jk, angle_ij-angle_jk);
						walls_state->AddTriangle(vi, vj, vk);
						walls_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
						walls_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);
					}
				}
			}
		}
		walls_state->RemovePolygon(topindex);
		walls_state->RemovePolygon(botindex2); // temporary */
	}
}

void GDSParse::HandlePath(iMaterialWrapper* tm, layers *thislayer)
{
	float X, Y, nextX=0.0, nextY=0.0, prevX=0.0, prevY=0.0;
	float firstX=0.0, firstY=0.0, secondX=0.0, secondY=0.0;
	int points = recordlen/8;
	int i;//, j, k;
	csVector3 vi, vj, vk;
	//double angle_ij, angle_jk;

	int pindex=0;
	int topindex=0, botindex=0;
	int topindex2=0, botindex2=0;

	if(currentwidth){
		/* FIXME - need to check for -ve value and then not scale (is this done in ParseWidth?) */
		X = units * (float)GetFourByteSignedInt();
		Y = units * (float)GetFourByteSignedInt();
		firstX = X;
		firstY = Y;
		fudge++;
		for(i=1; i<points+1; i++){
			if(i<points){
				nextX = units * (float)GetFourByteSignedInt();
				nextY = units * (float)GetFourByteSignedInt();
			}
			if(i==1){
				secondX = nextX;
				secondY = nextY;
			}
			/* FIXME - vertical going paths are broken */
			if(thislayer->height && thislayer->show && tm && fudge==4 && 0){
 				if(i<points){
					printf("width=%f\n", currentwidth);
/*					printf("x=%f, y=%f, nextx=%f, nexty=%f, \ 
							atan=%f\n",X,Y,nextX,nextY,atan2(X-nextX, nextY-Y) \
							);*/								
					printf("(%f, %f, %f)\n", \
							(float)(X + currentwidth * cos(atan2(X-nextX, nextY-Y))), \
							(float)(Y + currentwidth * sin(atan2(X-nextX, nextY-Y))), \
							(float)(units*thislayer->height));
					printf("(%f, %f, %f)\n", \
							(float)(nextX - currentwidth * cos(atan2(X-nextX, nextY-Y))), \
							(float)(nextY - currentwidth * sin(atan2(X-nextX, nextY-Y))), \
							(float)(units*(thislayer->height+thislayer->thickness)));
					fflush(stdout);
					walls_state->AddOutsideBox(\
						csVector3 ( \
						(float)(X + currentwidth * cos(atan2(X-nextX, nextY-Y))), \
						(float)(Y + currentwidth * sin(atan2(X-nextX, nextY-Y))), \
						(float)(units*thislayer->height)), \
						csVector3 ( \
						(float)(nextX - currentwidth * cos(atan2(X-nextX, nextY-Y))), \
						(float)(nextY - currentwidth * sin(atan2(X-nextX, nextY-Y))), \
						(float)(units*(thislayer->height+thislayer->thickness))) \
						);
					walls_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
					walls_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);
//					fprintf(outfile, "<%.2f, ", X + (float)currentwidth * cos(atan2(X-nextX, nextY-Y)));
//					fprintf(outfile, "%.2f>,", Y + (float)currentwidth * sin(atan2(X-nextX, nextY-Y)));
//					fprintf(outfile, "<%.2f, ", X - (float)currentwidth * cos(atan2(X-nextX, nextY-Y)));
//					fprintf(outfile, "%.2f>", Y - (float)currentwidth * sin(atan2(X-nextX, nextY-Y)));
				}else{
/*					walls_state->AddOutsideBox(\
						csVector3 (X + (float)currentwidth * cos(atan2(X-firstX, firstY-Y)), \
						Y + (float)currentwidth * sin(atan2(X-firstX, firstY-Y)), \
						thislayer.height), \
						csVector3 (X - (float)currentwidth * cos(atan2(X-firstX, firstY-Y)), \
						Y - (float)currentwidth * sin(atan2(X-firstX, firstY-Y)), \
						thislayer.height+thislayer.thickness) \
						);
					walls_state->SetPolygonMaterial (CS_POLYRANGE_LAST, tm);
					walls_state->SetPolygonTextureMapping (CS_POLYRANGE_LAST, 3);*/
//					fprintf(outfile, "<%.2f, ", X + (float)currentwidth * cos(atan2(X-firstX, firstY-Y)));
//					fprintf(outfile, "%.2f>,", Y + (float)currentwidth * sin(atan2(X-firstX, firstY-Y)));
//					fprintf(outfile, "<%.2f, ", X - (float)currentwidth * cos(atan2(X-firstX, firstY-Y)));
//					fprintf(outfile, "%.2f>,", Y - (float)currentwidth * sin(atan2(X-firstX, firstY-Y)));

//					fprintf(outfile, "<%.2f, ", X - (float)currentwidth * cos(atan2(firstX-secondX, secondY-firstY)));
//					fprintf(outfile, "%.2f>", Y - (float)currentwidth * sin(atan2(firstX-secondX, secondY-firstY)));

				}
			}
			prevX = X;
			X = nextX;
			prevY = Y;
			Y = nextY;
		}
	}
}
short GDSParse::GetBitArray()
{
	unsigned char byte1;

	fread(&byte1, 1, 1, infile);
	fread(&byte1, 1, 1, infile);

	recordlen -= 2;
	return 0;
}


double GDSParse::GetEightByteReal()
{
	unsigned char value;
	unsigned char b8, b2, b3, b4, b5, b6, b7;
	double sign=1.0;
	double exponent;
	double mant;

	fread(&value, 1, 1, infile);
	if(value & 128){
		value -= 128;
		sign = -1.0;
	}
	exponent = (double )value;
	exponent -= 64.0;
	mant=0.0;

	fread(&b2, 1, 1, infile);
	fread(&b3, 1, 1, infile);
	fread(&b4, 1, 1, infile);
	fread(&b5, 1, 1, infile);
	fread(&b6, 1, 1, infile);
	fread(&b7, 1, 1, infile);
	fread(&b8, 1, 1, infile);

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

	recordlen -= 8;

	return sign*(mant*pow(16.0,exponent));
}

long GDSParse::GetFourByteSignedInt()
{
	long value;
	fread(&value, 4, 1, infile);
	
	recordlen -= 4;
	return endian_swap_long(value);
}

short GDSParse::GetTwoByteSignedInt()
{
	short value;

	fread(&value, 2, 1, infile);

	recordlen -= 2;

	return endian_swap_short(value);
}

char *GDSParse::GetAsciiString()
{
	char *str=NULL;

	recordlen += recordlen%2;

	str = (char *)malloc(recordlen+1);
	if(!str){
		printf("Unable to allocate memory for ascii string (%d)\n", recordlen);
		fclose(infile);
		exit(1);
	}
	fread(str, 1, recordlen, infile);
	str[recordlen] = 0;
	recordlen = 0;

	return str;
}

long endian_swap_long(long value)
{
	_asm{
		mov eax,value
		bswap eax
		mov value,eax
	}
	return value;
}

short endian_swap_short(short value)
{
	_asm{
		mov ax,value
		xchg ah,al
		mov value,ax
	}
	return value;
}

