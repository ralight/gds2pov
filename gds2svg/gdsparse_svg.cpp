/*
 * File: gdsparse_svg.cpp
 * Author: Roger Light
 * Project: gdsto3d
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

GDSParse_svg::GDSParse_svg(GDSProcess *process, FILE *optr, bool generate_process) :
		GDSParse(process, generate_process),
		m_scale(100.0)
{
	// FIXME - check multiple output and output children first
	m_optr = optr;
	m_bounding_output = false;
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
}


GDSParse_svg::GDSParse_svg(GDSParse *parse, FILE *optr)
{
	m_optr = optr;
	m_bounding_output = false;
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;

	m_units = parse->GetUnits();
	m_process = parse->GetProcess();

	vector<GDSObject*> objects = parse->GetObjects();
	for(unsigned int i = 0; i < objects.size(); i++){
		m_objects.push_back(new GDSObject_svg(objects[i], m_optr));
	}
}


GDSParse_svg::~GDSParse_svg ()
{
}

GDSObject *GDSParse_svg::NewObject(std::string name)
{
	return new GDSObject_svg(name, m_optr);
}

void GDSParse_svg::OutputFooter()
{
	if(m_optr && !m_objects.empty()){
		struct _Boundary *boundary = GetBoundary();

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
		struct _Boundary *boundary = GetBoundary();
		float width = (boundary->xmax - boundary->xmin);
		float height = (boundary->ymax - boundary->ymin);

		for(unsigned int i = 0; i < m_objects.size(); i++){
			GDSObject_svg *obj = static_cast<GDSObject_svg*>(m_objects[i]);
			obj->SetScale(m_scale);
		}

		fprintf(m_optr, "<?xml version=\"1.0\" standalone=\"no\"?>\n");
		fprintf(m_optr, "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n");

		fprintf(m_optr, "<svg width=\"%.2f\" height=\"%.2f\" viewBox=\"0 0 %.2f %.2f\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n",
				width*20, height*20, width*m_scale, height*m_scale);

		/* Output layer texture information */

		ProcessLayer *layer;
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
