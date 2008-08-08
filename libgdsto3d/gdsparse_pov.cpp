/*
 * File: gdsparse_pov.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the POV-RAY output specific implementation of the GDSParse class.
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

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsparse_pov.h"
#include "gdsobject_pov.h"

extern int verbose_output;

GDSParse_pov::GDSParse_pov(GDSConfig *config, GDSProcess *process,
		bool bounding_output, std::string camfile, bool generate_process) :
		GDSParse(config, process, generate_process)
{
	m_bounding_output = bounding_output;
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
}


GDSParse_pov::GDSParse_pov(const GDSParse *parse)
{
	m_bounding_output = false; // FIXME
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;

	m_units = parse->m_units;
	m_process = parse->m_process;
	m_config = parse->m_config;

	for(unsigned int i = 0; i < parse->m_objects.size(); i++){
		m_objects.push_back(new GDSObject_pov(parse->m_objects[i]));
	}
}


GDSParse_pov::~GDSParse_pov ()
{
}


GDSObject *GDSParse_pov::NewObject(std::string name)
{
	return new GDSObject_pov(name);
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

		struct _Boundary *boundary = GetBoundary();
		float half_widthX = (boundary->xmax - boundary->xmin)/2;
		float half_widthY = (boundary->ymax - boundary->ymin)/2;
		float centreX = half_widthX + boundary->xmin;
		float centreY = half_widthY + boundary->ymin;

		float distance;
		if(half_widthX > half_widthY){
			distance = half_widthX * 1.8;
		}else{
			distance = half_widthY * 1.8;
		}

		fprintf(m_optr, "#declare sizeX = %.2f;\n", boundary->xmax - boundary->xmin);
		fprintf(m_optr, "#declare sizeY = %.2f;\n", boundary->ymax - boundary->ymin);
//		fprintf(m_optr, "#declare sizeZ = %.2f\n", boundary->ZMax - boundary->ZMin);

		fprintf(m_optr, "// TopLeft: %.2f, %.2f\n", boundary->xmin, boundary->ymax);
		fprintf(m_optr, "// TopRight: %.2f, %.2f\n", boundary->xmax, boundary->ymax);
		fprintf(m_optr, "// BottomLeft: %.2f, %.2f\n", boundary->xmin, boundary->ymin);
		fprintf(m_optr, "// BottomRight: %.2f, %.2f\n", boundary->xmax, boundary->ymin);
		fprintf(m_optr, "// Centre: %.2f, %.2f\n", centreX, centreY);

		float xmod = m_config->GetCameraPos()->xmod;
		float ymod = m_config->GetCameraPos()->ymod;
		float zmod = m_config->GetCameraPos()->zmod;

		/* _camfile is a possible camera include file. Depends on the -e option
		 * If it is null, use the normal camera else use the include */
		if(m_camfile != ""){
			switch(m_config->GetCameraPos()->boundarypos){
				case bpCentre:
					// Default camera angle = 67.38
					// Half of this is 33.69
					// tan(33.69) = 0.66666 = 1/1.5
					// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8
	
					fprintf(m_optr, "camera {\n\tlocation <%.2f,%.2f,%.2f>\n",
							centreX*xmod, centreY*ymod, -distance*zmod);
					break;
				case bpTopLeft:
					fprintf(m_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
							boundary->xmin*xmod, boundary->ymax*ymod, -distance*zmod);
					break;
				case bpTopRight:
					fprintf(m_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
							boundary->xmax*xmod, boundary->ymax*ymod, -distance*zmod);
					break;
				case bpBottomLeft:
					fprintf(m_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
							boundary->xmin*xmod, boundary->ymin*ymod, -distance*zmod);
					break;
				case bpBottomRight:
					fprintf(m_optr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
							boundary->xmax*xmod, boundary->ymin*ymod, -distance*zmod);
					break;
			}

			fprintf(m_optr, "\tsky <0,0,-1>\n"); //This fixes the look at rotation (hopefully)

			xmod = m_config->GetLookAtPos()->xmod;
			ymod = m_config->GetLookAtPos()->ymod;
			zmod = m_config->GetLookAtPos()->zmod;

			switch(m_config->GetLookAtPos()->boundarypos){
				case bpCentre:
					fprintf(m_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
							centreX*xmod, centreY*ymod, -distance*zmod);
					break;
				case bpTopLeft:
					fprintf(m_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
							boundary->xmin*xmod, boundary->ymax*ymod, -distance*zmod);
					break;
				case bpTopRight:
					fprintf(m_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
							boundary->xmax*xmod, boundary->ymax*ymod, -distance*zmod);
					break;
				case bpBottomLeft:
					fprintf(m_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
							boundary->xmin*xmod, boundary->ymin*ymod, -distance*zmod);
					break;
				case bpBottomRight:
					fprintf(m_optr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
							boundary->xmax*xmod, boundary->ymin*ymod, -distance*zmod);
				break;
			}
		}else{
			fprintf(m_optr, "#include %s\n", m_camfile.c_str());
		}

		if(m_config->GetLightPos()!=NULL){
			int count = m_config->GetLightCount();

			for(int i = 0; i < count; i++){
				Position *LightPos = m_config->GetLightPos(i);
				xmod = LightPos->xmod;
				ymod = LightPos->ymod;
				zmod = LightPos->zmod;

				switch(LightPos->boundarypos){
					case bpCentre:
						fprintf(m_optr, "light_source {<%.2f,%.2f,%.2f> White }\n",
								centreX*xmod, centreY*ymod, -distance*zmod);
						break;
					case bpTopLeft:
						fprintf(m_optr, "light_source {<%.2f,%.2f,%.2f> White }\n",
								boundary->xmin*xmod, boundary->ymax*ymod, -distance*zmod);
						break;
					case bpTopRight:
						fprintf(m_optr, "light_source {<%.2f,%.2f,%.2f> White }\n",
								boundary->xmax*xmod, boundary->ymax*ymod, -distance*zmod);
						break;
					case bpBottomLeft:
						fprintf(m_optr, "light_source {<%.2f,%.2f,%.2f> White }\n",
								boundary->xmin*xmod, boundary->ymin*ymod, -distance*zmod);
						break;
					case bpBottomRight:
						fprintf(m_optr, "light_source {<%.2f,%.2f,%.2f> White }\n",
								boundary->xmax*xmod, boundary->ymin*ymod, -distance*zmod);
						break;
				}
			}
		}else{
			fprintf(m_optr, "light_source {<%.2f,%.2f,%.2f> White }\n",
					centreX, centreY, -distance);
		}

		fprintf(m_optr, "background { color Black }\n");
		fprintf(m_optr, "global_settings { ambient_light rgb <%.2f,%.2f,%.2f> }\n",
				m_config->GetAmbient(), m_config->GetAmbient(), m_config->GetAmbient());

		/* Output layer texture information */
		ProcessLayer *layer = NULL;
		for(int i = 0; i < m_process->LayerCount(); i++){
			layer = m_process->GetLayer(i);
			if(layer->Show){
				if(!layer->Metal){
					fprintf(m_optr, "#declare t%s = pigment{rgbf <%.2f, %.2f, %.2f, %.2f>}\n",
							layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
				}else{
					fprintf(m_optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n",
							layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
				}
			}
		}
		if(layer && layer->Show){
			if(!layer->Metal){
				fprintf(m_optr, "#declare t%s = pigment{rgbf <%.2f, %.2f, %.2f, %.2f>}\n",
						layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
			}else{
				fprintf(m_optr, "#declare t%s = texture{pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA}}\n",
						layer->Name.c_str(), layer->Red, layer->Green, layer->Blue, layer->Filter);
			}
		}

		if(m_bounding_output){
			fprintf(m_optr, "box {<%.2f,%.2f,%.2f> <%.2f,%.2f,%.2f> texture { pigment { rgb <0.75, 0.75, 0.75> } } }",
					boundary->xmin, boundary->ymin, m_units*m_process->GetLowest(),
					boundary->xmax, boundary->ymax, m_units*m_process->GetHighest());
		}
	}
}


void GDSParse_pov::Decompose(bool value)
{
	for(unsigned int i = 0; i < m_objects.size(); i++){
		static_cast<GDSObject_pov*>(m_objects[i])->Decompose(value);
	}
}

