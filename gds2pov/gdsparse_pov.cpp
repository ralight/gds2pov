/*
 * File: gdsparse_pov.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the POV-RAY output specific implementation of the GDSParse class.
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


#include <cstdio>

#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsparse_pov.h"
#include "gdsobject_pov.h"

extern int verbose_output;

GDSParse_pov::GDSParse_pov(GDSProcess *process, FILE *optr,
		bool bounding_output, bool generate_process) :
		GDSParse(process, generate_process)
{
	m_optr = optr;
	m_bounding_output = bounding_output;
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
}


GDSParse_pov::GDSParse_pov(GDSParse *parse, FILE *optr)
{
	m_optr = optr;
	m_bounding_output = false; // FIXME
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
	m_camfile = "";

	m_units = parse->GetUnits();
	m_process = parse->GetProcess();

	unordered_map<std::string, GDSObject*> objects = parse->GetObjects();
	for(auto it=m_objects.begin(); it!=m_objects.end(); it++) {
		auto object = it->second;
		auto object_pov = new GDSObject_pov(object, m_optr);

		m_objects[it->first] = static_cast<GDSObject *>(object_pov);
	}
}


GDSParse_pov::~GDSParse_pov ()
{
}


GDSObject *GDSParse_pov::NewObject(std::string name)
{
	return new GDSObject_pov(name, m_optr);
}

void GDSParse_pov::OutputFooter()
{
	if(!m_bounding_output){
		if(m_topcellname.length() > 0){
			fprintf(m_optr, "object { str_%s }\n", m_topcellname.c_str());
		}else{
			if(!m_objects.empty()){
				fprintf(m_optr, "object { str_%s }\n", m_objects[0]->GetName().c_str());
			}
		}
	}
}

void GDSParse_pov::OutputHeader()
{
	if(m_optr && !m_objects.empty()){
		fprintf(m_optr, "#include \"colors.inc\"\n");
		fprintf(m_optr, "#include \"metals.inc\"\n");
		fprintf(m_optr, "#include \"transforms.inc\"\n");


		/* Output layer texture information */
		ProcessLayer *layer = NULL;
		for(unsigned int i = 0; i < m_process->LayerCount(); i++){
			layer = m_process->GetLayer(i);
			if(layer->Show){
				if(!layer->Metal){
					fprintf(m_optr, "#declare t%s = pigment{rgbt <%.2f, %.2f, %.2f, %.2f>}\n",
							layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
				}else{
					fprintf(m_optr, "#declare t%s = texture{pigment{rgbt <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n",
							layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
				}
			}
		}
		if(layer && layer->Show){
			if(!layer->Metal){
				fprintf(m_optr, "#declare t%s = pigment{rgbt <%.2f, %.2f, %.2f, %.2f>}\n",
						layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
			}else{
				fprintf(m_optr, "#declare t%s = texture{pigment{rgbt <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n",
						layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
			}
		}

		if(m_bounding_output){
			struct _Boundary *boundary = GetBoundary();
			fprintf(m_optr, "box {<%.2f,%.2f,%.2f> <%.2f,%.2f,%.2f> texture { pigment { rgb <0.75, 0.75, 0.75> } } }",
					boundary->xmin, boundary->ymin, m_units*m_process->GetLowest(),
					boundary->xmax, boundary->ymax, m_units*m_process->GetHighest());
		}
	}
}


void GDSParse_pov::Decompose(bool value)
{
	for(auto it=m_objects.begin(); it!=m_objects.end(); it++) {
		static_cast<GDSObject_pov*>(it->second)->Decompose(value);
	}
}
