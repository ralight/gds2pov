#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "process_cfg.h"

int ReadProcessFile(FILE *fptr, layers **all_layers, int *layer_count)
{
	int layerstart_cnt = 0;
	int layerend_cnt = 0;
	char line[1024];
	int current_line = 0;
	int current_element = -1;
	int i;

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

	if(!fptr){
		return 1;
	}

	fseek(fptr, 0, SEEK_SET);
	while(!feof(fptr) && fgets(line, 1024, fptr)){
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
		return 1;
	}

	(*layer_count) = layerstart_cnt;
	(*all_layers) = (layers *)calloc(layerstart_cnt, sizeof(layers));
	//(*all_layers) = (layers *)calloc(layerstart_cnt, sizeof(layers*));
	if(!(*all_layers)){
		printf("Could not allocate memory for process information.\n");
		return 1;
	}

	fseek(fptr, 0, SEEK_SET);
	while(!feof(fptr) && fgets(line, 1024, fptr)){
		current_line++;
		if(line[0]!='#'){
			if(strstr(line, "LayerStart:")){
				if(in_layer){
					printf("Error: LayerStart without LayerEnd not allowed. LayerEnd should appear before line %d.\n", current_line);
					return 1;
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
				//(*all_layers)[current_element] = calloc(1, sizeof(layers));
			}else if(strstr(line, "Layer:")){
				if(!in_layer){
					printf("Error: Layer definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_layer){
					printf("Error: Duplicate Layer definition on line %d.\n", current_line);
					return 1;
				}
				sscanf(line, "Layer: %d", &((*all_layers)[current_element].layer));
				got_layer = 1;
			}else if(strstr(line, "Height:")){
				if(!in_layer){
					printf("Error: Height definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_height){
					printf("Error: Duplicate Height definition on line %d.\n", current_line);
					return 1;
				}
				sscanf(line, "Height: %d", &((*all_layers)[current_element].height));
				got_height = 1;
			}else if(strstr(line, "Thickness:")){
				if(!in_layer){
					printf("Error: Thickness definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_thickness){
					printf("Error: Duplicate Thickness definition on line %d.\n", current_line);
					return 1;
				}
				sscanf(line, "Thickness: %d", &((*all_layers)[current_element].thickness));
				got_thickness = 1;
			}else if(strstr(line, "Colour:")){
				if(!in_layer){
					printf("Error: Colour definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_colour){
					printf("Error: Duplicate Colour definition on line %d.\n", current_line);
					return 1;
				}
				strncpy((*all_layers)[current_element].colour, &line[8], 200);
				for(i=strlen((*all_layers)[current_element].colour)-1; i>=0; i--){
					if((*all_layers)[current_element].colour[i] == '\n'){
						(*all_layers)[current_element].colour[i] = '\0';
						break;
					}
				}
				got_colour = 1;
			}else if(strstr(line, "Metal:")){
				if(!in_layer){
					printf("Error: Metal definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_metal){
					printf("Error: Duplicate Metal definition on line %d.\n", current_line);
					return 1;
				}
				sscanf(line, "Metal: %d", &((*all_layers)[current_element].metal));
				got_metal = 1;
			}else if(strstr(line, "Transparent:")){
				if(!in_layer){
					printf("Error: Transparent definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_transparent){
					printf("Error: Duplicate Transparent definition on line %d.\n", current_line);
					return 1;
				}
				sscanf(line, "Transparent: %d", &((*all_layers)[current_element].transparent));
				got_transparent = 1;
			}else if(strstr(line, "Show:")){
				if(!in_layer){
					printf("Error: Show definition outside of LayerStart and LayerEnd on line %d.\n", current_line);
					return 1;
				}
				if(got_show){
					printf("Error: Duplicate Show definition on line %d.\n", current_line);
					return 1;
				}
				sscanf(line, "Show: %d", &((*all_layers)[current_element].show));
				got_show = 1;
			}else if(strstr(line, "LayerEnd")){
				if(!in_layer){
					printf("Error: LayerEnd without LayerStart on line %d.\n", current_line);
				}
				if(!got_layer){
					printf("Error: LayerEnd without Layer on line %d.\n", current_line);
				}
				if(!got_height){
					printf("Error: LayerEnd without Height on line %d.\n", current_line);
				}
				if(!got_thickness){
					printf("Error: LayerEnd without Thickness on line %d.\n", current_line);
				}
				if(!got_colour){
					printf("Error: LayerEnd without Colour on line %d.\n", current_line);
				}
				in_layer = 0;
			}
		}
	}
	if(in_layer){
		printf("Error: Missing final LayerEnd.\n");
		return 1;
	}

	return 0;
}

