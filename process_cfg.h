#ifndef _PROCESS_CFG_H
#define _PROCESS_CFG_H

#include <string.h>

#ifdef WIN32
#define __BYTE_ORDER 1
#define __LITTLE_ENDIAN 1
#endif

typedef struct{
	int layer;
	int height;
	int thickness;
	int show;
	int metal;
	int transparent;
	char colour[200];
} layers;

int ReadProcessFile(FILE *fptr, layers **all_layers, int *layer_count);

// colour
// height
// thickness

//void layertable(short layer, layers *thislayer);

#endif

