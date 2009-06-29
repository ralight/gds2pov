/*
 * File: gdsparse_info.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the INFO-RAY output specific implementation of the GDSParse class.
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


#include <cstdio>
//#include <stdlib.h>
//#include <cmath>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsparse_info.h"
#include "gdsobject_info.h"
//#include "gds_globals.h"
//#include "gds2info.h"
//#include "gdstext.h"
//#include "gdspolygon.h"


extern int verbose_output;

GDSParse_info::GDSParse_info (class GDSConfig *config) : GDSParse(config, NULL)
{
	SetOutputOptions(false, false, true, false);
}

GDSParse_info::~GDSParse_info ()
{
}

//class GDSObject_info *GDSParse_info::NewObject(char *Name)
class GDSObject *GDSParse_info::NewObject(char *Name)
{
	return new class GDSObject_info(Name);
}

void GDSParse_info::OutputFooter()
{
	if(_topcellname){
		fprintf(_optr, "object { str_%s }\n", _topcellname);
	}else{
		if(_Objects->GetObjectRef(0)){
			//FIXME fprintf(_optr, "object { str_%s }\n", _Objects->GetObjectRef(0)->GetName());
			fprintf(_optr, "%p\n", _Objects);
		}
	}
}

void GDSParse_info::OutputHeader()
{
	if(_optr && _Objects){
		fprintf(_optr, "#include \"colors.inc\"\n");
		fprintf(_optr, "#include \"metals.inc\"\n");
		fprintf(_optr, "#include \"transforms.inc\"\n");

		struct _Boundary *Boundary = _Objects->GetBoundary();
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


		fprintf(_optr, "// TopLeft: %.2f, %.2f\n", Boundary->XMin, Boundary->YMax);
		fprintf(_optr, "// TopRight: %.2f, %.2f\n", Boundary->XMax, Boundary->YMax);
		fprintf(_optr, "// BottomLeft: %.2f, %.2f\n", Boundary->XMin, Boundary->YMin);
		fprintf(_optr, "// BottomRight: %.2f, %.2f\n", Boundary->XMax, Boundary->YMin);
		fprintf(_optr, "// Centre: %.2f, %.2f\n", centreX, centreY);

		float XMod = _config->GetCameraPos()->XMod;
		float YMod = _config->GetCameraPos()->YMod;
		float ZMod = _config->GetCameraPos()->ZMod;

		switch(_config->GetCameraPos()->boundarypos){
			case bpCentre:
				// Default camera angle = 67.38
				// Half of this is 33.69
				// tan(33.69) = 0.66666 = 1/1.5
				// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8

				fprintf(_optr, "camera {\n\tlocation <%.2f,%.2f,%.2f>\n", centreX*XMod, centreY*YMod, -distance*ZMod);
				break;
			case bpTopLeft:
				fprintf(_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpTopRight:
				fprintf(_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpBottomLeft:
				fprintf(_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMin*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
			case bpBottomRight:
				fprintf(_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n", Boundary->XMax*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
		}

		fprintf(_optr, "\tsky <0,0,-1>\n"); //This fixes the look at rotation (hopefully)

		XMod = _config->GetLookAtPos()->XMod;
		YMod = _config->GetLookAtPos()->YMod;
		ZMod = _config->GetLookAtPos()->ZMod;

		switch(_config->GetLookAtPos()->boundarypos){
			case bpCentre:
				fprintf(_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", centreX*XMod, centreY*YMod, -distance*ZMod);
				break;
			case bpTopLeft:
				fprintf(_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMin*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpTopRight:
				fprintf(_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMax*XMod, Boundary->YMax*YMod, -distance*ZMod);
				break;
			case bpBottomLeft:
				fprintf(_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMin*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
			case bpBottomRight:
				fprintf(_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n", Boundary->XMax*XMod, Boundary->YMin*YMod, -distance*ZMod);
				break;
		}

		if(_config->GetLightPos()!=NULL){
			Position dummypos;
			dummypos.Next = _config->GetLightPos();

			Position *LightPos = &dummypos;

			while(LightPos->Next){
				LightPos = LightPos->Next;
				XMod = LightPos->XMod;
				YMod = LightPos->YMod;
				ZMod = LightPos->ZMod;

				switch(LightPos->boundarypos){
					case bpCentre:
						fprintf(_optr, "light_source {<%.2f,%.2f,%.2f> White }\n", centreX*XMod, centreY*YMod, -distance*ZMod);
						break;
					case bpTopLeft:
						fprintf(_optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMin*XMod, Boundary->YMax*YMod, -distance*ZMod);
						break;
					case bpTopRight:
						fprintf(_optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMax*XMod, Boundary->YMax*YMod, -distance*ZMod);
						break;
					case bpBottomLeft:
						fprintf(_optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMin*XMod, Boundary->YMin*YMod, -distance*ZMod);
						break;
					case bpBottomRight:
						fprintf(_optr, "light_source {<%.2f,%.2f,%.2f> White }\n", Boundary->XMax*XMod, Boundary->YMin*YMod, -distance*ZMod);
						break;
				}
			}
		}else{
			fprintf(_optr, "light_source {<%.2f,%.2f,%.2f> White }\n", centreX, centreY, -distance);
		}

		fprintf(_optr, "background { color Black }\n");
		fprintf(_optr, "global_settings { ambient_light rgb <%.2f,%.2f,%.2f> }\n", _config->GetAmbient(), _config->GetAmbient(), _config->GetAmbient());

		/* Output layer texture information */
		class ProcessLayer *firstlayer;
		while(firstlayer && firstlayer->Next){
			if(firstlayer->Show){
				if(!firstlayer->Metal){
					fprintf(_optr, "#declare t%s = pigment{rgbf <%.2f, %.2f, %.2f, %.2f>}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
				}else{
					fprintf(_optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
				}
			}
			firstlayer = firstlayer->Next;
		}
		if(firstlayer->Show){
			if(!firstlayer->Metal){
				fprintf(_optr, "#declare t%s = pigment{rgbf <%.2f, %.2f, %.2f, %.2f>}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
			}else{
				fprintf(_optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n", firstlayer->Name, firstlayer->Red, firstlayer->Green, firstlayer->Blue, firstlayer->Filter);
			}
		}
	}
}

