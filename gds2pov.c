#include <stdio.h>
#include <stdlib.h>

#include "gds2pov.h"
#include "gds_types.h"
#include "gds_parse.h"
#include "gds_globals.h"
#include "process_cfg.h"

char *libname;
char *sname;
char *textstring;
short currentlayer;
float currentwidth;
element_type currentelement;
short currentpathtype;
short currenttexttype;
short currentpresentation;
short arrayrows, arraycols;
float units;
unsigned short currentstrans;
float currentangle;
parse_mode Mode;
char **StrNames;
char *CurrentStrName;
int StrCount;
int CurrentStr;

void debug_printf(char *string)
{
	//printf("%s\n", string);
}

int main(int argc, char *argv[])
{
	FILE *infile=NULL;
	FILE *outfile=NULL;
	layers *all_layers=NULL;
	int layer_count;
	int i;

	libname = NULL;
	sname = NULL;

	if(argc!=3){
		printf("Usage: gds2pov infile.gds outfile.pov\n");
		return 0;
	}

	/* FOR TESTING ReadProcessFile */
	infile = fopen("process.txt", "rt");
	if(!infile){
		printf("Unable to read %s\n", argv[1]);
		return 0;
	}

	if(ReadProcessFile(infile, &all_layers, &layer_count)==1){
		printf("Error reading the process information file.\n");
		if(all_layers){
			for(i=0; i<layer_count; i++){
				if((&all_layers)[i]){
					free((&all_layers)[i]);
				}
			}
			free(all_layers);
		}
		fclose(infile);
		exit(1);
	}

	fclose(infile);
	infile = NULL;
	/* END FOR TESTING ReadProcessFile */


	infile = fopen(argv[1], "rb");
	if(!infile){
		printf("Unable to read %s\n", argv[1]);
		return 0;
	}

	outfile = fopen(argv[2], "wt");
	if(!outfile){
		printf("Unable to write %s\n", argv[2]);
		fclose(infile);
		return 0;
	}

	fprintf(outfile, "#include \"colors.inc\"\n");
	fprintf(outfile, "#include \"metals.inc\"\n");
	fprintf(outfile, "#include \"transforms.inc\"\n");
	fprintf(outfile, "camera {\n\tlocation <22000, 70000, 18000>\n");
	fprintf(outfile, "\tlook_at <22000, 2000, 18000>\n}");
	fprintf(outfile, "background { color Black }\n");
	fprintf(outfile, "light_source { <2000, 2000, -30000> White }\n");
	fprintf(outfile, "light_source { <10000, 20000, -10000> White }\n");
	fprintf(outfile, "light_source { <30000, 20000, 10000> White }\n");
	GDStoPOV(infile, outfile, all_layers, layer_count);

	if(StrNames && StrNames[StrCount-1]){
		fprintf(outfile, "object { str_%s }\n", StrNames[StrCount-1]);
	}else{
		printf("Unable to find any structures!\n");
	}
	fclose(infile);
	fclose(outfile);

	if(libname){
		free(libname);
	}
	if(sname){
		free(sname);
	}
	if(all_layers){
		free(all_layers);
	}
	if(StrNames){
		for(i=0; i<StrCount; i++){
			if(StrNames[i]){
				free(StrNames[i]);
			}
		}
		free(StrNames);
	}
	return 0;
}

