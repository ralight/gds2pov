#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "process_cfg.h"

GDSProcess::GDSProcess(char *processfile)
{
	int layerstart_cnt = 0;
	int layerend_cnt = 0;
	char line[1024];
	int current_line = 0;
	int current_element = -1;
	int i;

	FILE *pptr = NULL;

	Count = 0;

	/* State variables */
	char in_layer = 0;
	char got_layer = 0;
	char got_height = 0;
	char got_thickness = 0;
	char got_colour = 0;
	char got_metal = 0;
	char got_transparent = 0;
	char got_show = 0;
	/* End State variables */

	struct ProcessLayer NewLayer;

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
				got_colour = 0;
				got_metal = 0;
				got_transparent = 0;
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
				sscanf(line, "Layer: %d", &NewLayer.layer));
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
				sscanf(line, "Height: %d", &NewLayer.height));
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
				sscanf(line, "Thickness: %d", &NewLayer.thickness));
				got_thickness = 1;
			}else if(strstr(line, "Colour:")){
				if(!in_layer){
					printf("Error: Colour definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_colour){
					printf("Error: Duplicate Colour definition on line %d.\n", current_line);
					return;
				}
				if(NewLayer.colour){
					delete NewLayer.colour;
					NewLayer.colour = NULL;
				}
				NewLayer.colour = new char[256];
				
				strncpy(NewLayer.colour, &line[8], 200);
				for(i=strlen(NewLayer.colour)-1; i>=0; i--){
					if(NewLayer.colour[i] == '\n'){
						NewLayer.colour[i] = '\0';
						break;
					}
				}
				got_colour = 1;
			}else if(strstr(line, "Metal:")){
				if(!in_layer){
					printf("Error: Metal definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_metal){
					printf("Error: Duplicate Metal definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Metal: %d", &NewLayer.metal);
				got_metal = 1;
			}else if(strstr(line, "Transparent:")){
				if(!in_layer){
					printf("Error: Transparent definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_transparent){
					printf("Error: Duplicate Transparent definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Transparent: %d", &NewLayer.transparent);
				got_transparent = 1;
			}else if(strstr(line, "Show:")){
				if(!in_layer){
					printf("Error: Show definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return;
				}
				if(got_show){
					printf("Error: Duplicate Show definition on line %d.\n", current_line);
					return;
				}
				sscanf(line, "Show: %d", &NewLayer.show);
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
				}else if(!got_colour){
					printf("Error: LayerEnd without Colour on line %d.\n", current_line);
				}else{
					AddLayer(&NewLayer)
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
		if(layer1->Colour){
			delete layer1->Colour;
		}
		if(layer1){
			delete layer1;
		}
		layer1 = layer2;
	}
	if(layer1->Name){
		delete layer1->Name;
	}
	if(layer1->Colour){
		delete layer1->Colour;
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

struct ProcessLayer *GDSProcess::GetLayer(char *Name)
{
}

int GDSProcess::LayerCount()
{
	return Count;
}

GDSProcess::AddLayer(struct ProcessLayer *NewLayer)
{
	struct ProcessLayer *layer;

	layer = FirstLayer;

	if(FirstLayer){
		while(layer->Next){
			layer = layer->Next;
		}
		layer->Next = new struct ProcessLayer;
		layer = layer->Next;
	}else{
		FirstLayer = new struct ProcessLayer;
		layer = FirstLayer;
	}

	layer->Name = new char[strlen(NewLayer->Name)+1];
	strncpy(Layer->Name, NewLayer->Name, strlen(NewLayer->Name)+1);
	layer->Layer = NewLayer->Layer;
	layer->Height = NewLayer->Height;
	layer->Thickness = NewLayer->Thickness;
	layer->Show = NewLayer->Show;
	layer->Metal = NewLayer->Metal;
	layer->Transparent = NewLayer->Transparent;
	layer->Colour = new char[strlen(NewLayer->Colour)+1];
	strncpy(Layer->Colour, NewLayer->Colour, strlen(NewLayer->Colour)+1);
}

