#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdsparse.h"
#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"

extern int verbose_output;

void GDSParse::Output3DSFooter()
{
	unsigned long filesize;

	fseek(optr, 0L, SEEK_END);
	filesize = ftell(optr);
	fseek(optr, 2L, SEEK_SET);
	fwrite(&filesize, 4, 1, optr);

	fseek(optr, 18, SEEK_SET);
	filesize -= 16;
	fwrite(&filesize, 4, 1, optr);
}

void GDSParse::Output3DSCamera(struct _Boundary *Boundary)
{
	float XMod = config->GetCameraPos()->XMod;
	float YMod = config->GetCameraPos()->YMod;
	float ZMod = config->GetCameraPos()->ZMod;
	unsigned short chunk;
	unsigned long chunklen;
	unsigned char tempbyte;
	float tempf;

	chunk = 0x4000; // Named object
	chunklen = 38 + 2 + 4 + strlen("Camera") + 1;
	fwrite(&chunk, 2, 1, optr);
	fwrite(&chunklen, 4, 1, optr);
	fprintf(optr, "%s", "Camera");
	tempbyte = 0;
	fwrite(&tempbyte, 1, 1, optr);
		
	chunk = 0x4700; // Camera
	fwrite(&chunk, 2, 1, optr);
	chunklen = 38;
	fwrite(&chunklen, 4, 1, optr);
		
	Output3DSCameraLightPos(Boundary, config->GetCameraPos()->boundarypos, XMod, YMod, ZMod);

	XMod = config->GetLookAtPos()->XMod;
	YMod = config->GetLookAtPos()->YMod;
	ZMod = config->GetLookAtPos()->ZMod;

	Output3DSCameraLightPos(Boundary, config->GetLookAtPos()->boundarypos, XMod, YMod, ZMod);

	tempf = 0.0; // bank angle
	fwrite(&tempf, 4, 1, optr);
	tempf = 20.0; // focal length mm
	fwrite(&tempf, 4, 1, optr);
}

void GDSParse::Output3DSLights(struct _Boundary *Boundary)
{
	float XMod;
	float YMod;
	float ZMod;
	int LightIndex = 0;
	char LightName[30];
	unsigned short chunk;
	unsigned long chunklen;
	unsigned char tempbyte;

	if(config->GetLightPos()!=NULL){
		Position dummypos;
		dummypos.Next = config->GetLightPos();

		Position *LightPos = &dummypos;

		while(LightPos->Next){
			LightIndex++;
			sprintf(LightName, "%s%d", "Light", LightIndex);

			chunk = 0x4000; // Named object
			chunklen = 18 + 6 + strlen(LightName) + 1;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);
			fprintf(optr, "%s", LightName);
			tempbyte = 0;
			fwrite(&tempbyte, 1, 1, optr);

			chunk = 0x4600; // Light chunk
			chunklen = 18;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);

			LightPos = LightPos->Next;

			XMod = LightPos->XMod;
			YMod = LightPos->YMod;
			ZMod = LightPos->ZMod;

			Output3DSCameraLightPos(Boundary, LightPos->boundarypos, XMod, YMod, ZMod);
		}
	}else{
		sprintf(LightName, "%s%d", "Light", LightIndex);
		chunk = 0x4000; // Named object
		chunklen = 18 + 6 + strlen(LightName) + 1;
		fwrite(&chunk, 2, 1, optr);
		fwrite(&chunklen, 4, 1, optr);
		fprintf(optr, "%s", LightName);
		tempbyte = 0;
		fwrite(&tempbyte, 1, 1, optr);

		chunk = 0x4600; // Light chunk
		chunklen = 18;
		fwrite(&chunk, 2, 1, optr);
		fwrite(&chunklen, 4, 1, optr);

		Output3DSCameraLightPos(Boundary, bpBottomRight, 1.0, 1.0, 1.0);
		// colour
	}
}

void GDSParse::Output3DSCameraLightPos(struct _Boundary *Boundary, BoundaryPos bp, float XMod, float YMod, float ZMod)
{
	float half_widthX = (Boundary->XMax - Boundary->XMin)/2;
	float half_widthY = (Boundary->YMax - Boundary->YMin)/2;
	float centreX = half_widthX + Boundary->XMin;
	float centreY = half_widthY + Boundary->YMin;
	float tempf;
	
	float distance;

	if(half_widthX > half_widthY){
		distance = half_widthX * 1.8;
	}else{
		distance = half_widthY * 1.8;
	}

	switch(bp){
		case bpCentre:
			// Default camera angle = 67.38
			// Half of this is 33.69
			// tan(33.69) = 0.66666 = 1/1.5
			// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8
			tempf = centreX*XMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = centreY*YMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = -distance*ZMod;
			fwrite(&tempf, 4, 1, optr);
			break;
		case bpTopLeft:
			tempf = Boundary->XMin*XMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = Boundary->YMax*YMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = -distance*ZMod;
			fwrite(&tempf, 4, 1, optr);
			break;
		case bpTopRight:
			tempf = Boundary->XMax*XMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = Boundary->YMax*YMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = -distance*ZMod;
			fwrite(&tempf, 4, 1, optr);
			break;
		case bpBottomLeft:
			tempf = Boundary->XMin*XMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = Boundary->YMin*YMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = -distance*ZMod;
			fwrite(&tempf, 4, 1, optr);
			break;
		case bpBottomRight:
			tempf = Boundary->XMax*XMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = Boundary->YMin*YMod;
			fwrite(&tempf, 4, 1, optr);
			tempf = -distance*ZMod;
			fwrite(&tempf, 4, 1, optr);
			break;
	}
}

void GDSParse::Output3DSMaterials()
{
	unsigned short chunk;
	unsigned long chunklen;
	unsigned char tempbyte;

	struct ProcessLayer *firstlayer;
	struct ProcessLayer dummylayer;
	dummylayer.Next = process->GetLayer();
	//firstlayer = process->GetLayer();
	firstlayer = &dummylayer;
	while(firstlayer->Next){
		firstlayer = firstlayer->Next;

		if(firstlayer->Show){
			chunk = 0xAFFF; // Material chunk
			chunklen = 6 + 6 + strlen(firstlayer->Name) + 1 + 24 + 24; // FIXME - length for metallic

			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);

			chunk = 0xA000; // Material name chunk
			chunklen = 6 + strlen(firstlayer->Name) + 1;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);
			fprintf(optr, "%s", firstlayer->Name);
			chunk = 0;
			fwrite(&chunk, 1, 1, optr);

			chunk = 0xA010; // Ambient colour 
			chunklen = 24;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);

			chunk = 0x0011; // Colour_24
			chunklen = 9;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Red*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Blue*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Green*255);
			fwrite(&tempbyte, 1, 1, optr);

			chunk = 0x0012; // Lin_Colour_24
			chunklen = 9;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Red*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Blue*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Green*255);
			fwrite(&tempbyte, 1, 1, optr);

			chunk = 0xA020; // Diffuse colour 
			chunklen = 24;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);

			chunk = 0x0011; // Colour_24
			chunklen = 9;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Red*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Blue*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Green*255);
			fwrite(&tempbyte, 1, 1, optr);

			chunk = 0x0012; // Lin_Colour_24
			chunklen = 9;
			fwrite(&chunk, 2, 1, optr);
			fwrite(&chunklen, 4, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Red*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Blue*255);
			fwrite(&tempbyte, 1, 1, optr);
			tempbyte = (unsigned char)(firstlayer->Green*255);
			fwrite(&tempbyte, 1, 1, optr);
			if(firstlayer->Metal){
				// FIXME - make reflective
			//	fprintf(optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
			}
		}
	}
}

void GDSParse::Output3DSHeader()
{
	unsigned short chunk;
	unsigned long  chunklen;
	unsigned long  version;

	if(optr){
		chunk = 0x4D4D; // MAIN Chunk
		fwrite(&chunk, 2, 1, optr);
		chunklen = 0;
		fwrite(&chunklen, 4, 1, optr);

		chunk = 0x0002; // Version chunk
		chunklen = 10;
		fwrite(&chunk, 2, 1, optr);
		fwrite(&chunklen, 4, 1, optr);
		version = 3;
		fwrite(&version, 4, 1, optr);

		chunk = 0x3D3D; // Editor chunk
		chunklen = 0;
		fwrite(&chunk, 2, 1, optr);
		fwrite(&chunklen, 4, 1, optr);

		chunk = 0x3D3E; // Mesh version chunk
		chunklen = 10;
		fwrite(&chunk, 2, 1, optr);
		fwrite(&chunklen, 4, 1, optr);
		chunklen = 3;
		fwrite(&chunklen, 4, 1, optr);

		struct _Boundary *Boundary = Objects->GetBoundary();
		Output3DSCamera(Boundary);
		Output3DSLights(Boundary);

		/*tempbyte = 222;
		fwrite(&tempbyte, 1, 1, optr);
		tempbyte = 173;
		fwrite(&tempbyte, 1, 1, optr);*/
		// FIXME
		//chunk = 0x1200; // background chunk
		//chunklen = 0;
		//fwrite(&chunk, 2, 1, optr);
		//fwrite(&chunklen, 4, 1, optr);
		//fprintf(optr, "background { color Black }\n");
		//
		//chunk = 0x2100; // ambient chunk
		//chunklen = 0;
		//fwrite(&chunk, 2, 1, optr);
		//fwrite(&chunklen, 4, 1, optr);
		//fprintf(optr, "global_settings { ambient_light rgb <%.2f,%.2f,%.2f> }\n", config->GetAmbient(), config->GetAmbient(), config->GetAmbient());

		/* Output layer texture information */
		Output3DSMaterials();
	}
}

