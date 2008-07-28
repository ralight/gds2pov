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


//#include <cstdio>
//#include <cstdlib>
//#include <cmath>

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

GDSParse_svg::GDSParse_svg (class GDSConfig *config, class GDSProcess *process, bool generate_process) : GDSParse(config, process, generate_process)
{
	_config = config;
	_scale = 100.0;
	_generate_process = generate_process;
	// FIXME - check multiple output and output children first
	SetOutputOptions(false, true, false, true);
}

GDSParse_svg::~GDSParse_svg ()
{
}

class GDSObject *GDSParse_svg::NewObject(std::string Name)
{
	return new class GDSObject_svg(Name);
}

void GDSParse_svg::OutputFooter()
{
	if(_optr && !_Objects.empty()){
		struct _Boundary *Boundary = GetBoundary();
		
		fprintf(_optr, "\t</defs>\n");
		if(_topcellname.length() > 0){
			fprintf(_optr, "\t<use x=\"%.2f\" y=\"%.2f\" width=\"100%\" height=\"100%\" xlink:href=\"#%s\"/>\n", -_scale*Boundary->XMin/2, -_scale*Boundary->YMin/2, _topcellname.c_str());
		}else{
			if(_Objects[0]){
				fprintf(_optr, "\t<use x=\"%.2f\" y=\"%.2f\" width=\"100%\" height=\"100%\" xlink:href=\"#%s\"/>\n", -_scale*Boundary->XMin/2, -_scale*Boundary->YMin/2, _Objects[0]->GetName().c_str());
		}else{
			}
		}
		fprintf(_optr, "</svg>");
	}
}

void GDSParse_svg::OutputHeader()
{
	if(_optr && !_Objects.empty()){
		struct _Boundary *Boundary = GetBoundary();
		float width = (Boundary->XMax - Boundary->XMin);
		float height = (Boundary->YMax - Boundary->YMin);
		GDSObject_svg *obj;

		for(unsigned int i = 0; i < _Objects.size(); i++){
			/* FIXME - use c++ style casts */
			obj = (GDSObject_svg *)_Objects[i];
			obj->SetScale(_scale);
		}

		fprintf(_optr, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
		fprintf(_optr, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

		fprintf(_optr, "<svg width=\"%.2f\" height=\"%.2f\" viewBox=\"0 0 %.2f %.2f\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n", width*20, height*20, width*_scale, height*_scale);

		/* Output layer texture information */

		class ProcessLayer *firstlayer;
		fprintf(_optr, "\t<style>\n");
		for(int i=0; i < _process->LayerCount(); i++){
			firstlayer = _process->GetLayer(i);
			if(firstlayer->Show){
				fprintf(_optr, "\t\t.%s { fill: #%02x%02x%02x; opacity:0.75; }\n", firstlayer->Name.c_str(), (int)(255*firstlayer->Red), (int)(255*firstlayer->Green), (int)(255*firstlayer->Blue));
			}
		}
		fprintf(_optr, "\t</style>\n");

		/* Finish styles */

		fprintf(_optr, "\t<title></title>");
		fprintf(_optr, "\t<desc></desc>\n");

		float distance;
		if(width/2 > height/2){
			distance = width * 1.8/2;
		}else{
			distance = height * 1.8/2;
		}

		fprintf(_optr, "\t<defs>\n");

	}
}

