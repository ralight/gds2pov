#include <stdio.h>
#include <string.h>

	/*
	** ProcessFile structure
	**
	** # at the start of a line is a comment
	**
	** StartLayer: Name
	** Layer: Number
	** Height: Number
	** Thickness: Number
	** Colour: <0, 0.5, 1>
	** Metal: 1/0
	** Transparent: 1/0
	** Show: 1/0
	** EndLayer
	*/


#include "process_cfg.h"

GDSProcess::GDSProcess(char *processfile)
{
	int layerstart_cnt = 0;
	int layerend_cnt = 0;
	char line[1024];
	int current_line = 0;
	int current_element = -1;

	FILE *pptr = NULL;

	Count = 0;

	/* State variables */
	char in_layer = 0;
	char got_layer = 0;
	char got_height = 0;
	char got_thickness = 0;
	char got_red = 0;
	char got_green = 0;
	char got_blue = 0;
	char got_filter = 0;
	char got_metal = 0;
	char got_show = 0;
	/* End State variables */

	FirstLayer = NULL;

	struct ProcessLayer NewLayer;

	pptr = fopen(processfile, "rt");
	
	if(!pptr){
		return;
	}

	while(!feof(pptr) && fgets(line, 1024, pptr)){
		if(strstr(line, "LayerStart")){			
			layerstart_cnt++;
		}else if(strstr(line, "LayerEnd")){
			layerend_cnt++;
		}
	}
	if(layerstart_cnt!=layerend_cnt){
		printf("Invalid process file. ");
		printf("There should be equal numbers of LayerStart and LayerEnd elements! ");
		printf("(%d and %d found respectively)\n", layerstart_cnt, layerend_cnt);
		return;
	}

	Count = layerstart_cnt;

	fseek(pptr, 0, SEEK_SET);
	while(!feof(pptr) && fgets(line, 1024, pptr)){
		current_line++;
		if(line[0]!='#'){
			if(strstr(line, "LayerStart:")){
				if(in_layer){
					printf("Error: LayerStart without LayerEnd not allowed. LayerEnd should appear before line %d.\n", current_line);
					return;
				}
				in_layer = 1;
				got_layer = 0;
				got_height = 0;
				got_thickness = 0;
				got_red = 0;
				got_green = 0;
				got_blue = 0;
				got_filter = 0;
				got_metal = 0;
				got_show = 0;				
				current_element++;

			}else if(strstr(line, "Layer:")){
				if(!in_layer){
					printf("Error: Layer definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_layer){
					printf("Error: Duplicate Layer definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Layer: %d", &NewLayer.Layer);
				got_layer = 1;
			}else if(strstr(line, "Height:")){
				if(!in_layer){
					printf("Error: Height definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_height){
					printf("Error: Duplicate Height definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Height: %f", &NewLayer.Height);
				got_height = 1;
			}else if(strstr(line, "Thickness:")){
				if(!in_layer){
					printf("Error: Thickness definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_thickness){
					printf("Error: Duplicate Thickness definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Thickness: %f", &NewLayer.Thickness);
				got_thickness = 1;
			}else if(strstr(line, "Red:")){
				if(!in_layer){
					printf("Error: Red definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_red){
					printf("Error: Duplicate Red definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Red: %f", &NewLayer.Red);
				got_red = 1;
			}else if(strstr(line, "Green:")){
				if(!in_layer){
					printf("Error: Green definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_green){
					printf("Error: Duplicate Green definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Green: %f", &NewLayer.Green);
				got_green = 1;
			}else if(strstr(line, "Blue:")){
				if(!in_layer){
					printf("Error: Blue definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_blue){
					printf("Error: Duplicate Blue definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Blue: %f", &NewLayer.Blue);
				got_blue = 1;
			}else if(strstr(line, "Filter:")){
				if(!in_layer){
					printf("Error: Filter definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_filter){
					printf("Error: Duplicate Filter definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Filter: %f", &NewLayer.Filter);
				got_filter = 1;
			}else if(strstr(line, "Metal:")){
				if(!in_layer){
					printf("Error: Metal definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_metal){
					printf("Error: Duplicate Metal definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Metal: %d", &NewLayer.Metal);
				got_metal = 1;
			}else if(strstr(line, "Show:")){
				if(!in_layer){
					printf("Error: Show definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_show){
					printf("Error: Duplicate Show definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Show: %d", &NewLayer.Show);
				got_show = 1;
			}else if(strstr(line, "LayerEnd")){
				if(!in_layer){
					printf("Error: LayerEnd without LayerStart on line %d.\n", current_line);
				}else if(!got_layer){
					printf("Error: LayerEnd without Layer on line %d.\n", current_line);
				}else if(!got_height){
					printf("Error: LayerEnd without Height on line %d.\n", current_line);
				}else if(!got_thickness){
					printf("Error: LayerEnd without Thickness on line %d.\n", current_line);
				}else if(!got_red){
					printf("Error: LayerEnd without Red on line %d.\n", current_line);
				}else if(!got_green){
					printf("Error: LayerEnd without Green on line %d.\n", current_line);
				}else if(!got_blue){
					printf("Error: LayerEnd without Blue on line %d.\n", current_line);
				}else if(!got_filter){
					printf("Error: LayerEnd without Filter on line %d.\n", current_line);
				}else{
					AddLayer(&NewLayer);
				}

				in_layer = 0;
			}
		}
	}
	if(in_layer){
		printf("Error: Missing final LayerEnd.\n");
		return;
	}
}

GDSProcess::~GDSProcess ()
{
	struct ProcessLayer *layer1;
	struct ProcessLayer *layer2;

	layer1 = FirstLayer;

	while(layer1->Next){
		layer2 = layer1->Next;
		if(layer1->Name){
			delete layer1->Name;
		}
		if(layer1){
			delete layer1;
		}
		layer1 = layer2;
	}
	if(layer1->Name){
		delete layer1->Name;
	}
	if(layer1){
		delete layer1;
	}
}

struct ProcessLayer *GDSProcess::GetLayer(int Number)
{
	struct ProcessLayer *layer;

	layer = FirstLayer;

	while(layer){
		if(layer->Layer == Number){
			return layer;
		}
		layer = layer->Next;
	}
	return NULL;
}

int GDSProcess::LayerCount()
{
	return Count;
}

void GDSProcess::AddLayer(struct ProcessLayer *NewLayer)
{
	struct ProcessLayer *layer;

	layer = FirstLayer;

	if(FirstLayer){
		while(layer->Next){
			layer = layer->Next;
		}
		layer->Next = new struct ProcessLayer;
		layer = layer->Next;
		layer->Next = NULL;
	}else{
		FirstLayer = new struct ProcessLayer;
		layer = FirstLayer;
		layer->Next = NULL;
	}

	//layer->Name = new char[strlen(NewLayer->Name)+1];
	//strncpy(layer->Name, NewLayer->Name, strlen(NewLayer->Name)+1);
	layer->Layer = NewLayer->Layer;
	layer->Height = NewLayer->Height;
	layer->Thickness = NewLayer->Thickness;
	layer->Show = NewLayer->Show;
	layer->Red = NewLayer->Red;
	layer->Green = NewLayer->Green;
	layer->Blue = NewLayer->Blue;
	layer->Filter = NewLayer->Filter;
	layer->Metal = NewLayer->Metal;
}

