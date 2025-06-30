/*
 * File: config_cfg.cpp
 * Author: Roger Light
 * Project: gds2x
 * $Id: config_cfg.cpp 302 2008-02-26 10:47:01Z roger $
 *
 * This parses the configuration file which contains camera and light
 * information.
 *
 * Copyright (C) 2004-2005 Roger Light.
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
#include <cstring>

#include "config_cfg.h"

GDSConfig::GDSConfig(std::string filename, std::string camfile) :
	m_processfile(""), m_font(""),
	m_ambient(1.2), m_scale(1.0), m_valid(true)
{
	m_camfile = camfile;
	m_camerapos.postype = ptCamera;
	m_lookatpos.postype = ptLookAt;

	if(filename != ""){
		ReadFile(filename);
	}
}

GDSConfig::~GDSConfig()
{
	while(!m_lights.empty()){
		delete m_lights[m_lights.size()-1];
		m_lights.pop_back();
	}
}

bool GDSConfig::IsValid()
{
	return m_valid;
}

float GDSConfig::GetAmbient()
{
	return m_ambient;
}

float GDSConfig::GetScale()
{
	return m_scale;
}

std::string GDSConfig::GetProcessFile()
{
	return m_processfile;
}

std::string GDSConfig::GetFont()
{
	return m_font;
}

Position *GDSConfig::GetLookAtPos()
{
	return &m_lookatpos;
}

Position *GDSConfig::GetCameraPos()
{
	return &m_camerapos;
}

Position *GDSConfig::GetLightPos()
{
	return m_lights[0];
}

Position *GDSConfig::GetLightPos(unsigned int index)
{
	if(index < m_lights.size()){
		return m_lights[index];
	}
	return NULL;
}

int GDSConfig::GetLightCount()
{
	return m_lights.size();
}

void GDSConfig::ReadFile(std::string configfile)
{
	int posstart_cnt = 0;
	int posend_cnt = 0;
	int globalstart_cnt = 0;
	int globalend_cnt = 0;
	char line[1024];
	int current_line = 0;

	PosType current_type = ptNone;

	FILE *cptr = NULL;

	bool in_global = false;
	bool got_ambient = false;
	bool got_processfile = false;
	bool got_font = false;
	bool got_scale = false;

	bool in_position = false;
	bool got_type = false;
	bool got_position = false;
	bool got_xmod = false;
	bool got_ymod = false;
	bool got_zmod = false;

	int i;

	cptr = fopen(configfile.c_str(), "rt");

	if(!cptr){
		fprintf(stderr, "Error: Unable to open config file \"%s\"\n", configfile.c_str());
		m_valid = false;
		return;
	}

	while(!feof(cptr) && fgets(line, 1024, cptr)){
		if(line[0]!='#'){
			if(strstr(line, "PositionStart")){
				posstart_cnt++;
			}else if(strstr(line, "PositionEnd")){
				posend_cnt++;
			}else if(strstr(line, "GlobalStart")){
				globalstart_cnt++;
			}else if(strstr(line, "GlobalEnd")){
				globalend_cnt++;
			}
		}
	}
	if(posstart_cnt!=posend_cnt){
		fprintf(stderr, "Invalid config file. ");
		fprintf(stderr, "There should be equal numbers of PositionStart and PositionEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", posstart_cnt, posend_cnt);
		m_valid = false;
		return;
	}

	if(globalstart_cnt!=globalend_cnt || globalstart_cnt > 1 || globalend_cnt > 1){
		fprintf(stderr, "Invalid config file. ");
		fprintf(stderr, "There should be either 1 or 0 of both of GlobalStart and GlobalEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", globalstart_cnt, globalend_cnt);
		m_valid = false;
		return;
	}

//	PosCount = posstart_cnt;

	fseek(cptr, 0, SEEK_SET);
	while(!feof(cptr) && fgets(line, 1024, cptr)){
		current_line++;
		if(line[0]!='#'){
			if(strstr(line, "GlobalStart")){
				if(in_position){
					fprintf(stderr, "Error: GlobalStart inside PositionStart on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				in_global = true;
				got_ambient = false;
				got_processfile = false;
			}else if(strstr(line, "Ambient:")){
				if(!in_global){
					fprintf(stderr, "Error: Ambient definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_ambient){
					fprintf(stderr, "Warning: Duplicate Ambient definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Ambient: %f", &m_ambient);
				}
				got_ambient = true;
			}else if(strstr(line, "Scale:")){
				if(!in_global){
					fprintf(stderr, "Error: Scale definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_scale){
					fprintf(stderr, "Warning: Duplicate Scale definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Scale: %f", &m_scale);
					if(m_scale<0.001){
						fprintf(stderr, "Warning: Scale is very small (<0.001)\n");
					}
				}
				got_scale = true;
			}else if(strstr(line, "ProcessFile:")){
				if(!in_global){
					fprintf(stderr, "Error: ProcessFile definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_processfile){
					fprintf(stderr, "Warning: Duplicate ProcessFile definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					m_processfile = line;
					m_processfile = m_processfile.substr(13);

					for(i=m_processfile.length()-1; i>=0; i--){
						if(m_processfile[i] == '\n'){
							m_processfile[i] = '\0';
							break;
						}
					}
				}
				got_processfile = true;
			}else if(strstr(line, "Font:")){
				if(!in_global){
					fprintf(stderr, "Error: Font definition outside of GlobalStart and GlobalEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_font){
					fprintf(stderr, "Warning: Duplicate Font definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					m_font = line;
					m_font = m_font.substr(6);
					for(i=m_font.length()-1; i>=0; i--){
						if(m_font[i] == '\n'){
							m_font[i] = '\0';
							break;
						}
					}
				}
				got_font = true;
			}else if(strstr(line, "GlobalEnd")){
				in_global = false;
			}else if(strstr(line, "PositionStart")){
				if(in_position){
					fprintf(stderr, "Error: PositionStart without PositionEnd not allowed. PositionEnd should appear before line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}else if(in_global){
					fprintf(stderr, "Error: PositionStart inside GlobalStart on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				in_position = true;
				got_type = false;
				got_position = false;
				got_xmod = false;
				got_ymod = false;
				got_zmod = false;
				current_type = ptNone;
			}else if(strstr(line, "Type:")){
				if(!in_position){
					fprintf(stderr, "Error: Type definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_type){
					fprintf(stderr, "Warning: Duplicate Type definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					if(strstr(line, "Type: Camera")){
						current_type = ptCamera;
					}else if(strstr(line, "Type: LookAt")){
						current_type = ptLookAt;
					}else if(strstr(line, "Type: Light")){
						current_type = ptLight;

						m_currentlight = new Position;
						m_lights.push_back(m_currentlight);
					}else{
						fprintf(stderr, "Error: Unknown position type \"%s\" on line %d of config file.\n", line, current_line);
						m_valid = false;
						fclose(cptr);
						return;
					}
					got_type = true;
				}
			}else if(strstr(line, "Position:")){
				if(!in_position){
					fprintf(stderr, "Error: Position definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_position){
					fprintf(stderr, "Warning: Duplicate Position definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					BoundaryPos thispos;
					if(strstr(line, "Position: Centre")){
						thispos = bpCentre;
					}else if(strstr(line, "Position: TopLeft")){
						thispos = bpTopLeft;
					}else if(strstr(line, "Position: TopRight")){
						thispos = bpTopRight;
					}else if(strstr(line, "Position: BottomLeft")){
						thispos = bpBottomLeft;
					}else if(strstr(line, "Position: BottomRight")){
						thispos = bpBottomRight;
					}else{
						fprintf(stderr, "Error: Unknown Position \"%s\" on line %d of config file.\n", line, current_line);
						m_valid = false;
						fclose(cptr);
						return;
					}
					switch(current_type){
						case ptCamera:
							m_camerapos.boundarypos = thispos;
							break;
						case ptLookAt:
							m_lookatpos.boundarypos = thispos;
							break;
						case ptLight:
							if(m_currentlight){
								m_currentlight->boundarypos = thispos;
							}else{
								fprintf(stderr, "Error: Position found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								m_valid = false;
								fclose(cptr);
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_position = true;
				}
			}else if(strstr(line, "XMod:")){
				if(!in_position){
					fprintf(stderr, "Error: XMod definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_xmod){
					fprintf(stderr, "Error: Duplicate XMod definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					switch(current_type){
						case ptCamera:
							sscanf(line, "XMod: %f", &m_camerapos.xmod);
							break;
						case ptLookAt:
							sscanf(line, "XMod: %f", &m_lookatpos.xmod);
							break;
						case ptLight:
							if(m_currentlight){
								sscanf(line, "XMod: %f", &m_currentlight->xmod);
							}else{
								fprintf(stderr, "Error: XMod found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								m_valid = false;
								fclose(cptr);
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_xmod = true;
				}
			}else if(strstr(line, "YMod:")){
				if(!in_position){
					fprintf(stderr, "Error: YMod definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_ymod){
					fprintf(stderr, "Error: Duplicate YMod definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					switch(current_type){
						case ptCamera:
							sscanf(line, "YMod: %f", &m_camerapos.ymod);
							break;
						case ptLookAt:
							sscanf(line, "YMod: %f", &m_lookatpos.ymod);
							break;
						case ptLight:
							if(m_currentlight){
								sscanf(line, "YMod: %f", &m_currentlight->ymod);
							}else{
								fprintf(stderr, "Error: YMod found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								m_valid = false;
								fclose(cptr);
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_ymod = true;
				}
			}else if(strstr(line, "ZMod:")){
				if(!in_position){
					fprintf(stderr, "Error: ZMod definition outside of PositionStart and PositionEnd on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(!got_type){
					fprintf(stderr, "Error: Type must be defined before any other elements in a Position block.\n");
					m_valid = false;
					fclose(cptr);
					return;
				}
				if(got_zmod){
					fprintf(stderr, "Error: Duplicate ZMod definition on line %d of config file. Ignoring new definition.\n", current_line);
				}else{
					switch(current_type){
						case ptCamera:
							sscanf(line, "ZMod: %f", &m_camerapos.zmod);
							break;
						case ptLookAt:
							sscanf(line, "ZMod: %f", &m_lookatpos.zmod);
							break;
						case ptLight:
							if(m_currentlight){
								sscanf(line, "ZMod: %f", &m_currentlight->zmod);
							}else{
								fprintf(stderr, "Error: ZMod found but LastLight not initialised (this shouldn't happen, please contact the author)\n");
								m_valid = false;
								fclose(cptr);
								return;
							}
							break;
						case ptNone:
						default:
							fprintf(stderr, "Error: Unknown position type found (this shouldn't happen, please contact the author)\n");
							break;
					}
					got_zmod = true;
				}
			}else if(strstr(line, "PositionEnd")){
				if(!in_position){
					fprintf(stderr, "Error: PositionEnd without PositionStart on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}else if(!got_type){
					fprintf(stderr, "Error: PositionEnd without Type on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}else if(!got_position){
					fprintf(stderr, "Error: PositionEnd without Position on line %d of config file.\n", current_line);
					m_valid = false;
					fclose(cptr);
					return;
				}
				in_position = false;
			}
		}
	}
	fclose(cptr);
}


void GDSConfig::OutputToFile(FILE *fptr, struct GDS2X::Boundary *boundary)
{
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

	fprintf(fptr, "#declare sizeX = %.2f;\n", boundary->xmax - boundary->xmin);
	fprintf(fptr, "#declare sizeY = %.2f;\n", boundary->ymax - boundary->ymin);

	fprintf(fptr, "// TopLeft: %.2f, %.2f\n", boundary->xmin, boundary->ymax);
	fprintf(fptr, "// TopRight: %.2f, %.2f\n", boundary->xmax, boundary->ymax);
	fprintf(fptr, "// BottomLeft: %.2f, %.2f\n", boundary->xmin, boundary->ymin);
	fprintf(fptr, "// BottomRight: %.2f, %.2f\n", boundary->xmax, boundary->ymin);
	fprintf(fptr, "// Centre: %.2f, %.2f\n", centreX, centreY);

	float xmod = this->GetCameraPos()->xmod;
	float ymod = this->GetCameraPos()->ymod;
	float zmod = this->GetCameraPos()->zmod;

	/* m_camfile is a possible camera include file. Depends on the -e option
	 * If it is null, use the normal camera else use the include */
	if(m_camfile == ""){
		switch(this->GetCameraPos()->boundarypos){
			case bpCentre:
				// Default camera angle = 67.38
				// Half of this is 33.69
				// tan(33.69) = 0.66666 = 1/1.5
				// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8

				fprintf(fptr, "camera {\n\tlocation <%.2f,%.2f,%.2f>\n",
						centreX*xmod, centreY*ymod, -distance*zmod);
				break;
			case bpTopLeft:
				fprintf(fptr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
						boundary->xmin*xmod, boundary->ymax*ymod, -distance*zmod);
				break;
			case bpTopRight:
				fprintf(fptr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
						boundary->xmax*xmod, boundary->ymax*ymod, -distance*zmod);
				break;
			case bpBottomLeft:
				fprintf(fptr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
						boundary->xmin*xmod, boundary->ymin*ymod, -distance*zmod);
				break;
			case bpBottomRight:
				fprintf(fptr, "camera {\n\tlocation <%.2f, %.2f, %.2f>\n",
						boundary->xmax*xmod, boundary->ymin*ymod, -distance*zmod);
				break;
		}

		fprintf(fptr, "\tsky <0,0,-1>\n"); //This fixes the look at rotation (hopefully)

		xmod = this->GetLookAtPos()->xmod;
		ymod = this->GetLookAtPos()->ymod;
		zmod = this->GetLookAtPos()->zmod;

		switch(this->GetLookAtPos()->boundarypos){
			case bpCentre:
				fprintf(fptr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
						centreX*xmod, centreY*ymod, -distance*zmod);
				break;
			case bpTopLeft:
				fprintf(fptr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
						boundary->xmin*xmod, boundary->ymax*ymod, -distance*zmod);
				break;
			case bpTopRight:
				fprintf(fptr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
						boundary->xmax*xmod, boundary->ymax*ymod, -distance*zmod);
				break;
			case bpBottomLeft:
				fprintf(fptr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
						boundary->xmin*xmod, boundary->ymin*ymod, -distance*zmod);
				break;
			case bpBottomRight:
				fprintf(fptr, "\tlook_at <%.2f,%.2f,%.2f>\n}\n",
						boundary->xmax*xmod, boundary->ymin*ymod, -distance*zmod);
			break;
		}
	}else{
		fprintf(fptr, "#include \"%s\"\n", m_camfile.c_str());
	}

	if(this->GetLightPos()!=NULL){
		int count = this->GetLightCount();

		for(int i = 0; i < count; i++){
			Position *LightPos = this->GetLightPos(i);
			xmod = LightPos->xmod;
			ymod = LightPos->ymod;
			zmod = LightPos->zmod;

			switch(LightPos->boundarypos){
				case bpCentre:
					fprintf(fptr, "light_source {<%.2f,%.2f,%.2f> rgb 1 }\n",
							centreX*xmod, centreY*ymod, -distance*zmod);
					break;
				case bpTopLeft:
					fprintf(fptr, "light_source {<%.2f,%.2f,%.2f> rgb 1 }\n",
							boundary->xmin*xmod, boundary->ymax*ymod, -distance*zmod);
					break;
				case bpTopRight:
					fprintf(fptr, "light_source {<%.2f,%.2f,%.2f> rgb 1 }\n",
							boundary->xmax*xmod, boundary->ymax*ymod, -distance*zmod);
					break;
				case bpBottomLeft:
					fprintf(fptr, "light_source {<%.2f,%.2f,%.2f> rgb 1 }\n",
							boundary->xmin*xmod, boundary->ymin*ymod, -distance*zmod);
					break;
				case bpBottomRight:
					fprintf(fptr, "light_source {<%.2f,%.2f,%.2f> rgb 1 }\n",
							boundary->xmax*xmod, boundary->ymin*ymod, -distance*zmod);
					break;
			}
		}
	}else{
		fprintf(fptr, "light_source {<%.2f,%.2f,%.2f> rgb 1 }\n",
				centreX, centreY, -distance);
	}

	fprintf(fptr, "background { color rgb 0 }\n");
	fprintf(fptr, "global_settings { ambient_light rgb <%.2f,%.2f,%.2f> }\n",
			this->GetAmbient(), this->GetAmbient(), this->GetAmbient());
}
