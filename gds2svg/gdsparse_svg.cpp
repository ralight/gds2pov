/*
 * File: gdsparse_svg.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the SVG output specific implementation of the GDSParse class.
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


//#include <stdio.h>
//#include <stdlib.h>
//#include <math.h>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsparse_svg.h"
#include "gdsobject_svg.h"
//#include "gds_globals.h"
//#include "gds2svg.h"
//#include "gdstext.h"
//#include "gdspolygon.h"


extern int verbose_output;

GDSParse_svg::GDSParse_svg (class GDSConfig *config, class GDSProcess *process) : GDSParse(config, process)
{
	_config = config;
	// FIXME - check multiple output and output children first
	SetOutputOptions(false, true, false, true);
}

GDSParse_svg::~GDSParse_svg ()
{
}

//class GDSObject_svg *GDSParse_svg::NewObject(char *Name)
class GDSObject *GDSParse_svg::NewObject(char *Name)
{
	return new class GDSObject_svg(Name);
}

void GDSParse_svg::OutputFooter()
{
	fprintf(_optr, "\t</defs>\n");
	if(_topcellname){
		fprintf(_optr, "\t<use x=\"0\" y=\"0\" xlink:href=\"#%s\"/>\n", _topcellname);
	}else{
		if(_Objects->GetObjectRef(0)){
			fprintf(_optr, "\t<use x=\"0\" y=\"0\" xlink:href=\"#%s\"/>\n", _Objects->GetObjectRef(0)->GetName());
		}
	}
	fprintf(_optr, "</svg>");
}

void GDSParse_svg::OutputHeader()
{
	if(_optr && _Objects){
		struct _Boundary *Boundary = _Objects->GetBoundary();
		float width = (Boundary->XMax - Boundary->XMin);
		float height = (Boundary->YMax - Boundary->YMin);
		int i;
		GDSObject_svg *obj;

		for(i = 0; i < _Objects->GetCount(); i++){
			obj = (GDSObject_svg *)_Objects->GetObjectRef(i);
			obj->SetInitialOffset(-Boundary->XMin, -Boundary->YMin);
			obj->SetScale(100);
		}

		fprintf(_optr, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
		fprintf(_optr, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

		fprintf(_optr, "<svg width=\"10cm\" height=\"10cm\" viewBox=\"0 0 %.2f %.2f\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", width*150, height*150);

		/* Output layer texture information */

		struct ProcessLayer *firstlayer;
		firstlayer = _process->GetLayer();
		fprintf(_optr, "\t<style>\n");
		while(firstlayer){
			if(firstlayer->Show){
				fprintf(_optr, "\t\t.%s { fill: #%02x%02x%02x; opacity:0.5; }\n", firstlayer->Name, (int)(255*firstlayer->Red), (int)(255*firstlayer->Green), (int)(255*firstlayer->Blue));
			}
			firstlayer = firstlayer->Next;
		}
		fprintf(_optr, "\t</style>\n");

		/* Finish styles */

		fprintf(_optr, "\t<title>Example Units</title>");
		fprintf(_optr, "\t<desc>Illustrates various units options</desc>\n");

		float centreX = width/2 + Boundary->XMin;
		float centreY = height/2 + Boundary->YMin;

		float distance;
		if(width/2 > height/2){
			distance = width * 1.8/2;
		}else{
			distance = height * 1.8/2;
		}

		fprintf(_optr, "\t<defs>\n");

	}
}

