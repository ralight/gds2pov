#include <stdio.h>
#include <string>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"
#include "gdsparse_pov.h"

#define GDS2POV_VERSION "0.8"

extern int verbose_output;
bool decompose;

void printusage()
{
	printf("gds2pov  version %s\n", GDS2POV_VERSION);
	printf("Copyright (C) 2004,2005 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
	printf("gds2pov comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2pov freely\nas described in the readme.txt distributed with this file.\n\n");
	printf("gds2pov is a program for converting a GDS2 file to a POV-Ray scene file.\n\n");
	printf("Usage: gds2pov [-b] [-c config.txt] [-d] [-h] [-i input.gds] [-o output.pov] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
	printf("Options\n");
	printf(" -b\t\tOutput bounding box instead of layout to allow easier and\n\t\tquicker placing of the camera\n");
	printf(" -c\t\tSpecify config file\n");
	printf(" -d\t\tDecompose polygons into triangles (use mesh2 object instead of prism)\n");
	printf(" -h\t\tDisplay this helps\n");
	printf(" -i\t\tInput GDS2 file (stdin if not specified)\n");
	printf(" -o\t\tOutput POV file (stdout if not specified)\n");
	printf(" -p\t\tSpecify process file\n");
	printf(" -q\t\tQuiet output\n");
	printf(" -t\t\tSpecify top cell name\n");
	printf(" -v\t\tVerbose output\n\n");
	printf("See http://www.atchoo.org/gdsto3d/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output = 1;
	bool bounding_output = false;
	decompose = false;

	if(argc>15){
		fprintf(stderr, "Error: Invalid number of arguments.\n\n");
		printusage();
		return 1;
	}

	char *gdsfile=NULL;
	char *povfile=NULL;

	char *configfile=NULL;
	char *processfile=NULL;
	char *topcell=NULL;

	for(int i=1; i<argc; i++){
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
			}else if(strncmp(argv[i], "-h", strlen("-h"))==0){
				printusage();
				return 0;
			}else if(strncmp(argv[i], "-i", strlen("-i"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -i switch given but no input file specified.\n\n");
					printusage();
					return 1;
				}else{
					gdsfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-o", strlen("-o"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -o switch given but no output file specified.\n\n");
					printusage();
					return 1;
				}else{
					povfile = argv[i+1];
				}
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

	FILE *iptr;
	if(gdsfile){
		iptr = fopen(gdsfile, "rb");
	}else{
		iptr = stdin;
	}
	if(iptr){
		class GDSParse_pov *Parser = new class GDSParse_pov(config, process, bounding_output);
		if(!Parser->Parse(iptr)){
			FILE *optr;
			if(povfile){
				optr = fopen(povfile, "wt");
			}else{
				optr = stdout;
			}

			if(optr){
				Parser->Output(optr, topcell, true, false, bounding_output);
				if(optr != stdout){
					fclose(optr);
				}
			}else{
				fprintf(stderr, "Error: Unable to open %s.\n", povfile);
			}
		}

		if(iptr != stdin){
			fclose(iptr);
		}

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

