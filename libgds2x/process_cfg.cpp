/*
 * File: process_cfg.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is used to parse the process configuration file.
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

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <sstream>
#include <string>


	/*
	** ProcessFile structure
	**
	** # at the start of a line is a comment
	**
	** StartLayer: Name
	** Layer: Number
	** Datatype: Number
	** Height: Number
	** Thickness: Number
	** Colour: #rrggbb
	** Metal: 1/0
	** Transparent: 1/0
	** Show: 1/0
	** EndLayer
	*/

#include "process_cfg.h"
#include "gds_globals.h"

using namespace GDS2X;

Process::Process() : m_valid(true)
{
}

Process::~Process ()
{
	while(!m_layers.empty()){
		delete m_layers[m_layers.size()-1];
		m_layers.pop_back();
	}
}

void Process::Parse(std::string processfile)
{
	int layerstart_cnt = 0;
	int layerend_cnt = 0;
	char line[1024];

	int current_line = 0;
	int current_element = -1;

	/* State variables */
	bool in_layer = false;
	bool got_layer = false;
	bool got_datatype = false;
	bool got_height = false;
	bool got_thickness = false;
	bool got_colour = false;
	bool got_red = false;
	bool got_green = false;
	bool got_blue = false;
	bool got_filter = false;
	bool got_metal = false;
	bool got_show = false;
	/* End State variables */
	bool showing;

	ProcessLayer NewLayer;

	FILE *pptr = NULL;

	pptr = fopen(processfile.c_str(), "rt");

	if(!pptr){
		fprintf(stderr, "Unable to open process file \"%s\".\n", processfile.c_str());
		m_valid = false;
		return;
	}

	while(!feof(pptr) && fgets(line, 1024, pptr)){
		if(line[0]!='#'){
			if(strstr(line, "LayerStart")){
				layerstart_cnt++;
			}else if(strstr(line, "LayerEnd")){
				layerend_cnt++;
			}
		}
	}
	if(layerstart_cnt!=layerend_cnt){
		fprintf(stderr, "Invalid process file. ");
		fprintf(stderr, "There should be equal numbers of LayerStart and LayerEnd elements! ");
		fprintf(stderr, "(%d and %d found respectively)\n", layerstart_cnt, layerend_cnt);
		m_valid = false;
		fclose(pptr);
		return;
	}

	fseek(pptr, 0, SEEK_SET);
	while(!feof(pptr) && fgets(line, 1024, pptr)){
		current_line++;
		if(line[0]!='#'){
			if(strstr(line, "LayerStart:")){
				if(in_layer){
					fprintf(stderr, "Error: LayerStart without LayerEnd not allowed. LayerEnd should appear before line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				in_layer = true;
				got_layer = false;
				got_datatype = false;
				got_height = false;
				got_thickness = false;
				got_colour = false;
				got_red = false;
				got_green = false;
				got_blue = false;
				got_filter = false;
				got_metal = false;
				got_show = false;
				current_element++;

				NewLayer.Name = (line+strlen("LayerStart: ")); /* FIXME - probably need to trim EOL */
				int p2 = NewLayer.Name.find_last_not_of(std::string(" \r\n"));
				int p1 = NewLayer.Name.find_first_not_of(std::string(" \r\n"));
				if (p1 == (int)std::string::npos) p1 = 0;
				NewLayer.Name =  NewLayer.Name.substr(p1, (p2-p1)+1);
				std::replace(NewLayer.Name.begin(), NewLayer.Name.end(), '-', '_');

				NewLayer.Layer = 0;
				NewLayer.Datatype = -1;
				NewLayer.Height = 0.0;
				NewLayer.Thickness = 0.0;
				NewLayer.Red = 0.0;
				NewLayer.Green = 0.0;
				NewLayer.Blue = 0.0;
				NewLayer.Filter = 0.0;
				NewLayer.Metal = false;
				NewLayer.Show = false;
			}else if(strstr(line, "Layer:")){
				if(!in_layer){
					fprintf(stderr, "Error: Layer definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_layer){
					fprintf(stderr, "Warning: Duplicate Layer definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Layer: %d", &NewLayer.Layer);
					got_layer = true;
				}
			}else if(strstr(line, "Datatype:")){
				if(!in_layer){
					fprintf(stderr, "Error: Datatype definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_datatype){
					fprintf(stderr, "Warning: Duplicate Datatype definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Datatype: %d", &NewLayer.Datatype);
					got_datatype = true;
				}
			}else if(strstr(line, "Height:")){
				if(!in_layer){
					fprintf(stderr, "Error: Height definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_height){
					fprintf(stderr, "Warning: Duplicate Height definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Height: %f", &NewLayer.Height);
					got_height = true;
				}
			}else if(strstr(line, "Thickness:")){
				if(!in_layer){
					fprintf(stderr, "Error: Thickness definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_thickness){
					fprintf(stderr, "Warning: Duplicate Thickness definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Thickness: %f", &NewLayer.Thickness);
					got_thickness = true;
				}
			}else if(strstr(line, "Colour:")){
				if(!in_layer){
					fprintf(stderr, "Error: Colour definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_colour){
					fprintf(stderr, "Warning: Duplicate Colour definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					unsigned int filter, red, green, blue;
					if(sscanf(line, "Colour: #%02x%02x%02x%02x", &red, &green, &blue, &filter) == 4){
						NewLayer.Filter = (float)filter/255.0;
					}else if(sscanf(line, "Colour: #%02x%02x%02x", &red, &green, &blue) == 3){
						/* Ok */
					}else{
						fprintf(stderr, "Error: Invalid Colour format. Expected #ffrrggbb or #rrggbb.\n");
						m_valid = false;
						fclose(pptr);
						return;
					}
					NewLayer.Red = (float)red/255.0;
					NewLayer.Green = (float)green/255.0;
					NewLayer.Blue = (float)blue/255.0;
					got_colour = true;
				}
			}else if(strstr(line, "Red:")){
				if(!in_layer){
					fprintf(stderr, "Error: Red definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_red){
					fprintf(stderr, "Warning: Duplicate Red definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Red: %f", &NewLayer.Red);
					got_red = true;
				}
			}else if(strstr(line, "Green:")){
				if(!in_layer){
					fprintf(stderr, "Error: Green definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_green){
					fprintf(stderr, "Warning: Duplicate Green definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Green: %f", &NewLayer.Green);
					got_green = true;
				}
			}else if(strstr(line, "Blue:")){
				if(!in_layer){
					fprintf(stderr, "Error: Blue definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_blue){
					fprintf(stderr, "Warning: Duplicate Blue definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Blue: %f", &NewLayer.Blue);
					got_blue = true;
				}
			}else if(strstr(line, "Filter:")){
				if(!in_layer){
					fprintf(stderr, "Error: Filter definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_filter){
					fprintf(stderr, "Warning: Duplicate Filter definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Filter: %f", &NewLayer.Filter);
					got_filter = true;
				}
			}else if(strstr(line, "Metal:")){
				if(!in_layer){
					fprintf(stderr, "Error: Metal definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_metal){
					fprintf(stderr, "Warning: Duplicate Metal definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					int met;
					sscanf(line, "Metal: %d", &met);
					if(met){
						NewLayer.Metal = true;
					}else{
						NewLayer.Metal = false;
					}
					got_metal = true;
				}
			}else if(strstr(line, "Show:")){
				if(!in_layer){
					fprintf(stderr, "Error: Show definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}
				if(got_show){
					fprintf(stderr, "Warning: Duplicate Show definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					int show;
					sscanf(line, "Show: %d", &show);
					if(show){
						NewLayer.Show = true;
					}else{
						NewLayer.Show = false;
					}

					got_show = true;
				}
			}else if(strstr(line, "LayerEnd")){
				showing = NewLayer.Show;
				if(!in_layer){
					fprintf(stderr, "Error: LayerEnd without LayerStart on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}else if(!got_layer){
					fprintf(stderr, "Error: LayerEnd without Layer on line %d of process file.\n", current_line);
					m_valid = false;
					fclose(pptr);
					return;
				}else if(!got_height){
					//v_printf(1, "Error: LayerEnd without Height on line %d of process file.\n", current_line);
					showing = false;
				}else if(!got_thickness){
//					v_printf(1, "Error: LayerEnd without Thickness on line %d of process file.\n", current_line);
					showing = false;
				}
				AddLayer(&NewLayer);
				if(!showing){
					if(NewLayer.Datatype == -1){
						v_printf(1, "Notice: Not showing layer %d (all datatypes)\n", NewLayer.Layer);
					}else{
						v_printf(1, "Notice: Not showing layer %d datatype %d\n", NewLayer.Layer, NewLayer.Datatype);
					}
				}

				in_layer = false;
			}
		}
	}
	fclose(pptr);
	v_printf(1, "\n");
}

ProcessLayer *Process::GetLayer(int Number, int Datatype)
{
	if(Number == -1) return NULL;

	for(unsigned int i = 0; i < m_layers.size(); i++){
		if(m_layers[i]->Layer == Number && m_layers[i]->Datatype == -1){
			return m_layers[i];
		}else if(m_layers[i]->Layer == Number && m_layers[i]->Datatype == Datatype){
			return m_layers[i];
		}
	}
	return NULL;
}

ProcessLayer *Process::GetLayer(std::string Name)
{
	for(unsigned int i = 0; i < m_layers.size(); i++){
		if(m_layers[i]->Name == Name){
			return m_layers[i];
		}
	}
	return NULL;
}

ProcessLayer *Process::GetLayer()
{
	if(!m_layers.empty()){
		return m_layers[0];
	}
	return NULL;
}

ProcessLayer *Process::GetLayer(unsigned int index)
{
	if(index < m_layers.size()){
		return m_layers[index];
	}else{
		return NULL;
	}
}

unsigned int Process::LayerCount()
{
	return m_layers.size();
}


ProcessLayer *Process::AddLayer(int Layer, int Datatype)
{
	ProcessLayer NewLayer;
	std::stringstream sName;

	sName << "Layer-" << Layer << ":" << Datatype;
	NewLayer.Name = sName.str();
	NewLayer.Layer = Layer;
	NewLayer.Datatype = Datatype;
	NewLayer.Height = 0.0;
	NewLayer.Thickness = 0.0;
	NewLayer.Red = 0.0;
	NewLayer.Green = 0.0;
	NewLayer.Blue = 0.0;
	NewLayer.Filter = 0.0;
	NewLayer.Metal = false;
	NewLayer.Show = false;

	return AddLayer(&NewLayer);
}


ProcessLayer *Process::AddLayer(ProcessLayer *NewLayer)
{
	ProcessLayer *layer = new ProcessLayer;

	layer->Name = NewLayer->Name;
	layer->Layer = NewLayer->Layer;
	layer->Datatype = NewLayer->Datatype;
	layer->Height = NewLayer->Height;
	layer->Thickness = NewLayer->Thickness;
	layer->Show = NewLayer->Show;
	layer->Red = NewLayer->Red;
	layer->Green = NewLayer->Green;
	layer->Blue = NewLayer->Blue;
	layer->Filter = NewLayer->Filter;
	layer->Metal = NewLayer->Metal;

	m_layers.push_back(layer);

	return layer;
}

bool Process::IsValid()
{
	return m_valid;
}

float Process::GetHighest()
{
	float Highest = -10000.0;

	for(unsigned int i = 0; i < m_layers.size(); i++){
		if(m_layers[i]->Height + m_layers[i]->Thickness > Highest && m_layers[i]->Show){
			Highest = m_layers[i]->Height + m_layers[i]->Thickness;
		}
	}
	return Highest;
}

float Process::GetLowest()
{
	float Lowest = 10000.0;

	for(unsigned int i = 0; i < m_layers.size(); i++){
		if(m_layers[i]->Height < Lowest && m_layers[i]->Show){
			Lowest = m_layers[i]->Height;
		}
	}
	return Lowest;
}


bool Process::RemoveLayer(unsigned int index)
{
	if(index < m_layers.size()){
		m_layers.erase(m_layers.begin()+index);
		return true;
	}else{
		return false;
	}
}

bool Process::Save(std::string filename)
{
	FILE *fptr = NULL;

	if(filename == "") return false;

	fptr = fopen(filename.c_str(), "wt");
	if(!fptr) return false;

	for(unsigned int i = 0; i < m_layers.size(); i++){
		fprintf(fptr, "LayerStart: %s\n", m_layers[i]->Name.c_str());
		fprintf(fptr, "Layer: %d\n", m_layers[i]->Layer);
		fprintf(fptr, "Height: %f\n", m_layers[i]->Height);
		fprintf(fptr, "Thickness: %f\n", m_layers[i]->Thickness);
		fprintf(fptr, "Red: %f\n", m_layers[i]->Red);
		fprintf(fptr, "Green: %f\n", m_layers[i]->Green);
		fprintf(fptr, "Blue: %f\n", m_layers[i]->Blue);
		fprintf(fptr, "Filter: %f\n", m_layers[i]->Filter);
		if(m_layers[i]->Metal){
			fprintf(fptr, "Metal: 1\n");
		}else{
			fprintf(fptr, "Metal: 0\n");
		}
		if(m_layers[i]->Show){
			fprintf(fptr, "Show: 1\n");
		}else{
			fprintf(fptr, "Show: 0\n");
		}
		fprintf(fptr, "LayerEnd\n\n");
	}
	fclose(fptr);

	return true;
}

void Process::SwapLayers(unsigned int a, unsigned int b)
{
	if(a < m_layers.size() && b < m_layers.size() && a != b){
		ProcessLayer *tmp;

		tmp = m_layers[a];
		m_layers[a] = m_layers[b];
		m_layers[b] = tmp;
	}
}

void ProcessLayer::HexColour(char *buf, size_t buflen)
{
	snprintf(buf, buflen, "%02x%02x%02x", (int )floor(255.0*Red), (int )floor(255.0*Green), (int )floor(255.0*Blue));
}
