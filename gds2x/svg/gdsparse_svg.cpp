/*
 * File: gdsparse_svg.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the SVG output specific implementation of the GDSParse class.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */


#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsparse_svg.h"
#include "gdsobject_svg.h"

extern int verbose_output;

GDSParse_svg::GDSParse_svg(GDS2X::Process *process, GDS2X::option_map_t &options) :
		GDS2X::Parse(process, false),
		m_scale(100.0)
{
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
	m_format = options["format"];
	m_outfile = options["outfile"];
	m_topcellname = options["topcell"];

	if(options["outfile"] != ""){
		m_optr = fopen(options["outfile"].c_str(), "wt");
		if(!m_optr){
			fprintf(stderr, "Error: Unable to open %s.\n", options["outfile"].c_str());
		}
	}else{
		m_optr = stdout;
	}
}


GDSParse_svg::~GDSParse_svg ()
{
	if(m_optr != stdout){
		fclose(m_optr);
	}
}

GDS2X::Object *GDSParse_svg::NewObject(std::string name)
{
	return new GDSObject_svg(name, m_optr);
}

void GDSParse_svg::OutputFooter()
{
	if(m_optr && !m_objects.empty()){
		struct GDS2X::Boundary *boundary = GetBoundary();

		fprintf(m_optr, "\t</defs>\n");
		if(m_topcellname.length() > 0){
			fprintf(m_optr, "\t<use x=\"%.2f\" y=\"%.2f\" width=\"100%%\" height=\"100%%\" xlink:href=\"#%s\"/>\n",
					-m_scale*boundary->xmin/2, -m_scale*boundary->ymin/2, m_topcellname.c_str());
		}else{
			if(m_objects[0]){
				fprintf(m_optr, "\t<use x=\"%.2f\" y=\"%.2f\" width=\"100%%\" height=\"100%%\" xlink:href=\"#%s\"/>\n",
						-m_scale*boundary->xmin/2, -m_scale*boundary->ymin/2, m_objects[0]->GetName().c_str());
		}else{
			}
		}
		fprintf(m_optr, "</svg>");
	}
}

void GDSParse_svg::OutputHeader()
{
	if(m_optr && !m_objects.empty()){
		struct GDS2X::Boundary *boundary = GetBoundary();
		float width = (boundary->xmax - boundary->xmin);
		float height = (boundary->ymax - boundary->ymin);

		for(auto it=m_objects.begin(); it!=m_objects.end(); it++) {
			GDSObject_svg *obj = static_cast<GDSObject_svg*>(it->second);
			obj->SetScale(m_scale);
		}

		fprintf(m_optr, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
		fprintf(m_optr, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

		fprintf(m_optr, "<svg width=\"%.2f\" height=\"%.2f\" viewBox=\"0 0 %.2f %.2f\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n",
				width*20, height*20, width*m_scale, height*m_scale);

		/* Output layer texture information */

		GDS2X::ProcessLayer *layer;
		fprintf(m_optr, "\t<style>\n");
		for(unsigned int i = 0; i < m_process->LayerCount(); i++){
			layer = m_process->GetLayer(i);
			if(layer->Show){
				fprintf(m_optr, "\t\t.%s { fill: #%02x%02x%02x; opacity:0.75; }\n",
						layer->Name.c_str(), (int)(255*layer->Red), (int)(255*layer->Green), (int)(255*layer->Blue));
				fprintf(m_optr, "\t\t.s%s { fill: none; stroke: #%02x%02x%02x; opacity:0.75; stroke-miterlimit:2.5;stroke-linejoin:miter}\n",
						layer->Name.c_str(), (int)(255*layer->Red), (int)(255*layer->Green), (int)(255*layer->Blue));
			}
		}
		fprintf(m_optr, "\t</style>\n");

		/* Finish styles */

		fprintf(m_optr, "\t<title></title>");
		fprintf(m_optr, "\t<desc></desc>\n");

		float distance;
		if(width/2 > height/2){
			distance = width * 1.8/2;
		}else{
			distance = height * 1.8/2;
		}

		fprintf(m_optr, "\t<defs>\n");

	}
}
