#include <stdio.h>
#include <string.h>

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
	Valid = true;

	/* State variables */
	bool in_layer = false;
	bool got_layer = false;
	bool got_datatype = false;
	bool got_height = false;
	bool got_thickness = false;
	bool got_red = false;
	bool got_green = false;
	bool got_blue = false;
	bool got_filter = false;
	bool got_metal = false;
	bool got_show = false;
	/* End State variables */
	bool showing;

	FirstLayer = NULL;

	struct ProcessLayer NewLayer;
	NewLayer.Name = NULL;

	pptr = fopen(processfile, "rt");
	
	if(!pptr){
		fprintf(stderr, "Unable to open process file \"%s\".\n", processfile);
		Valid = false;
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
		Valid = false;
		return;
	}

	Count = layerstart_cnt;

	fseek(pptr, 0, SEEK_SET);
	while(!feof(pptr) && fgets(line, 1024, pptr)){
		current_line++;
		if(line[0]!='#'){
			if(strstr(line, "LayerStart:")){
				if(in_layer){
					fprintf(stderr, "Error: LayerStart without LayerEnd not allowed. LayerEnd should appear before line %d of process file.\n", current_line);
					Valid = false;
					return;
				}
				in_layer = true;
				got_layer = false;
				got_datatype = false;
				got_height = false;
				got_thickness = false;
				got_red = false;
				got_green = false;
				got_blue = false;
				got_filter = false;
				got_metal = false;
				got_show = false;
				current_element++;

				if(NewLayer.Name){
					delete NewLayer.Name;
					NewLayer.Name = NULL;
				}
				NewLayer.Name = new char[strlen(line)-strlen("LayerStart: ")+1];
//				strncpy(NewLayer.Name, line+strlen("LayerStart: "), strlen(line)-strlen("LayerStart: "));
				strcpy(NewLayer.Name, line+strlen("LayerStart: "));
				NewLayer.Name[strlen(NewLayer.Name)-1] = '\0';
				NewLayer.Layer = 0;
				NewLayer.Datatype = -1;
				NewLayer.Height = 0.0;
				NewLayer.Thickness = 0.0;
				NewLayer.Red = 0.0;
				NewLayer.Green = 0.0;
				NewLayer.Blue = 0.0;
				NewLayer.Filter = 0.0;
				NewLayer.Metal = 0;
				NewLayer.Show = false;
				NewLayer.Next = NULL;
			}else if(strstr(line, "Layer:")){
				if(!in_layer){
					fprintf(stderr, "Error: Layer definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					Valid = false;
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
					Valid = false;
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
					Valid = false;
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
					Valid = false;
					return;
				}
				if(got_thickness){
					fprintf(stderr, "Warning: Duplicate Thickness definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Thickness: %f", &NewLayer.Thickness);
					got_thickness = true;
				}
			}else if(strstr(line, "Red:")){
				if(!in_layer){
					fprintf(stderr, "Error: Red definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					Valid = false;
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
					Valid = false;
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
					Valid = false;
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
					Valid = false;
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
					Valid = false;
					return;
				}
				if(got_metal){
					fprintf(stderr, "Warning: Duplicate Metal definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Metal: %d", &NewLayer.Metal);
					got_metal = true;
				}
			}else if(strstr(line, "Show:")){
				if(!in_layer){
					fprintf(stderr, "Error: Show definition outside of LayerStart and LayerEnd on line %d of process file.\n", current_line);
					Valid = false;
					return;
				}
				if(got_show){
					fprintf(stderr, "Warning: Duplicate Show definition on line %d of process file. Ignoring new definition.\n", current_line);
				}else{
					sscanf(line, "Show: %d", &NewLayer.Show);
					got_show = true;
				}
			}else if(strstr(line, "LayerEnd")){
				showing = NewLayer.Show;
				if(!in_layer){
					fprintf(stderr, "Error: LayerEnd without LayerStart on line %d of process file.\n", current_line);
					Valid = false;
					return;
				}else if(!got_layer){
					fprintf(stderr, "Error: LayerEnd without Layer on line %d of process file.\n", current_line);
					Valid = false;
					return;
				}else if(!got_height){
					//printf("Error: LayerEnd without Height on line %d of process file.\n", current_line);
					showing = false;
				}else if(!got_thickness){
//					printf("Error: LayerEnd without Thickness on line %d of process file.\n", current_line);
					showing = false;
				}
				AddLayer(&NewLayer);
					if(!showing){
						if(NewLayer.Datatype == -1){
							printf("Notice: Not showing layer %d (all datatypes)\n", NewLayer.Layer);
						}else{
							printf("Notice: Not showing layer %d datatype %d\n", NewLayer.Layer, NewLayer.Datatype);
						}
					}

				if(NewLayer.Name){
					delete NewLayer.Name;
					NewLayer.Name = NULL;
				}
				in_layer = false;
			}
		}
	}
	printf("\n");
}

GDSProcess::~GDSProcess ()
{
	if(FirstLayer){
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
}

struct ProcessLayer *GDSProcess::GetLayer(int Number, int Datatype)
{
	struct ProcessLayer *layer;

	layer = FirstLayer;

	while(layer){
		if(layer->Layer == Number && layer->Datatype == -1){
			return layer;
		}else if(layer->Layer == Number && layer->Datatype == Datatype){
			return layer;
		}
		layer = layer->Next;
	}
	return NULL;
}

struct ProcessLayer *GDSProcess::GetLayer()
{
	return FirstLayer;
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

	layer->Name = NULL;
	layer->Name = new char[strlen(NewLayer->Name)+1];
	strcpy(layer->Name, NewLayer->Name);
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
}

int GDSProcess::IsValid()
{
	return Valid;
}

float GDSProcess::GetHighest()
{
	float Highest = -10000.0;
	struct ProcessLayer *layer;

	layer = FirstLayer;
	if(FirstLayer){
		while(layer->Next){
			layer = layer->Next;
			if(layer->Height + layer->Thickness > Highest && layer->Show){
				Highest = layer->Height + layer->Thickness;
			}
		}
		if(layer->Height + layer->Thickness > Highest && layer->Show){
			Highest = layer->Height + layer->Thickness;
		}
	}
	return Highest;
}

float GDSProcess::GetLowest()
{
	float Lowest = 10000.0;
	struct ProcessLayer *layer;

	layer = FirstLayer;
	if(FirstLayer){
		while(layer->Next){
			layer = layer->Next;
			if(layer->Height < Lowest && layer->Show){
				Lowest = layer->Height;
			}
		}
		if(layer->Height < Lowest && layer->Show){
			Lowest = layer->Height;
		}
	}
	return Lowest;
}



