#include <stdio.h>
#include <string>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"
#include "gdsparse_pov.h"

#define VERSION "0.8"

extern int verbose_output;
bool decompose;

void printusage()
{
		printf("gds2pov  version %s\n", VERSION);
		printf("Copyright (C) 2004,2005 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
		printf("gds2pov comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2pov freely\nas described in the readme.txt distributed with this file.\n\n");
		printf("gds2pov is a program for converting a GDS2 file to a number of 3D formats.\n\n");
		printf("Usage: gds2pov input.gds output.pov [-b] [-c config.txt] [-d] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
		printf("Options\n");
		printf(" -b\t\tOutput bounding box instead of layout to allow easier and\n\t\tquicker placing of the camera\n");
		printf(" -c\t\tSpecify config file\n");
		printf(" -d\t\tDecompose polygons into triangles when using pov output format\n");
		printf(" -p\t\tSpecify process file\n -q\t\tQuiet output\n -t\t\tSpecify top cell name\n");
		printf(" -v\t\tVerbose output\n\n");
		printf("See http://www.atchoo.org/gdsto3d/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output = 1;
	bool bounding_output = false;
	decompose = false;

	if(argc<3 || argc>13){
		printf("Error: Invalid number of arguments.\n\n");
		printusage();
		return 1;
	}

	char *gdsfile=argv[1];
	char *povfile=argv[2];

	char *configfile=NULL;
	char *processfile=NULL;
	char *topcell=NULL;

	for(int i=3; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-b", strlen("-b"))==0){
				bounding_output = true;
			}else if(strncmp(argv[i], "-c", strlen("-c"))==0){

				if(i==argc-1){
					printf("Error: -c switch given but no config file specified.\n\n");
					printusage();
					return 1;
				}else{
					configfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-d", strlen("-d"))==0){
				decompose = true;
			}else if(strncmp(argv[i], "-p", strlen("-p"))==0){
				if(i==argc-1){
					printf("Error: -p switch given but no process file specified.\n\n");
					printusage();
					return 1;
				}else{
					processfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-q", strlen("-q"))==0){
				verbose_output--;
			}else if(strncmp(argv[i], "-t", strlen("-t"))==0){
				if(i==argc-1){
					printf("Error: -t switch given but no top cell specified.\n\n");
					printusage();
					return 1;
				}else{
					topcell = argv[i+1];
				}
			}else if(strncmp(argv[i], "-v", strlen("-v"))==0){
				verbose_output++;
			}else{
				printusage();
				return 1;
			}
		//}else{
		// Assume it is a process/config file specified on a previous arg
		}
	}

	class GDSConfig *config=NULL;

	if(configfile){
		config = new GDSConfig(configfile);
	}else{
		config = new GDSConfig(); // Start with default positions
	}
	if(!config){
		fprintf(stderr, "Error: Out of memory.\n");
	}else if(!config->IsValid()){
		fprintf(stderr, "Error: %s is not a valid config file.\n", configfile);
		delete config;
		return -1;
	}

	class GDSProcess *process=NULL;
	/* 
	** Order of precedence for process.txt:
	** -p switch (given as an argument to this function)
	** Specified in config file
	** Use process.txt if none others specified.
	*/
	if(processfile == NULL){
		if(config->GetProcessFile()!=NULL){
			processfile = config->GetProcessFile();
		}else{
			processfile = new char[13];
			strncpy(processfile, "process.txt", strlen("process.txt")+1);
		}
	}
	process = new GDSProcess(processfile);
	process->Parse(processfile);
	if(!process){
		fprintf(stderr, "Error: Out of memory.\n");
		delete config;
		delete process;
		return -1;
	}else if(!process->IsValid()){
		fprintf(stderr, "Error: %s is not a valid process file\n");
		delete config;
		delete process;
		return -1;
	}else if(process->LayerCount()==0){
		fprintf(stderr, "Error: No layers found in \"%s\".\n", processfile);
		delete config;
		delete process;
		return -1;
	}

	FILE *iptr = fopen(gdsfile, "rb");
	if(iptr){
		class GDSParse_pov *Parser = new class GDSParse_pov(config, process, bounding_output);
		if(!Parser->Parse(iptr)){
			FILE *optr = fopen(povfile, "wt");
			if(optr){
				Parser->Output(optr, topcell, true, false, bounding_output);
				fclose(optr);
			}else{
				fprintf(stderr, "Error: Unable to open %s.\n", povfile);
			}
		}

		fclose(iptr);

		delete Parser;
		delete config;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile);
		delete config;
		delete process;
		return -1;
	}
	return 0;
}

