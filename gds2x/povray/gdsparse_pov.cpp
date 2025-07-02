/*
 * File: gdsparse_pov.cpp
 * Author: Roger Light
 * Project: gds2x
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
#include "config_cfg.h"
#include "gdsparse.h"
#include "gdsparse_pov.h"
#include "gdsobject_pov.h"


GDSParse_pov::GDSParse_pov(GDS2X::Process *process, GDS2X::option_map_t &options) :
		GDS2X::Parse(process, options["generate_process"] == "true")
{
	m_allow_multiple_output = false;
	m_bounding_output = (options["bounding_output"] == "true");
	m_camfile = options["camerafile"];
	m_format = options["format"];
	m_outfile = options["outfile"];
	m_output_children_first = true;
	m_topcellname = options["topcell"];
	m_use_outfile = true;


	if(options["outfile"] != ""){
		m_optr = fopen(options["outfile"].c_str(), "wt");
		if(!m_optr){
			fprintf(stderr, "Error: Unable to open %s.\n", options["outfile"].c_str());
		}
	}else{
		m_optr = stdout;
	}

	std::string configfile = options["configfile"];
	if(configfile.length() > 0){
		m_config = new GDSConfig(configfile, m_camfile);
	}else{
		m_config = new GDSConfig();
	}
	if(!m_config->IsValid()){
		fprintf(stderr, "Error: %s is not a valid config file.\n", configfile.c_str());
	}
}


GDSParse_pov::~GDSParse_pov ()
{
	delete m_config;

	if(m_optr != stdout){
		fclose(m_optr);
	}
}


GDS2X::Object *GDSParse_pov::NewObject(std::string name)
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
		m_config->OutputToFile(m_optr, GetBoundary());

		fprintf(m_optr, "#include \"colors.inc\"\n");
		fprintf(m_optr, "#include \"metals.inc\"\n");
		fprintf(m_optr, "#include \"transforms.inc\"\n");


		/* Output layer texture information */
		GDS2X::ProcessLayer *layer = NULL;
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
			struct GDS2X::Boundary *boundary = GetBoundary();
			fprintf(m_optr, "box {<%.2f,%.2f,%.2f> <%.2f,%.2f,%.2f> texture { pigment { rgb <0.75, 0.75, 0.75> } } }",
					boundary->xmin, boundary->ymin, m_units*m_process->GetLowest(),
					boundary->xmax, boundary->ymax, m_units*m_process->GetHighest());
		}
	}
}
