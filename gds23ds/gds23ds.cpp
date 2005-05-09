#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "gds23ds.h"
#include "gdsparse.h"
#include "gds_globals.h"
#include "process_cfg.h"

#define GDS23DS_VERSION "0.8"

/*extern int verbose_output;
extern bool quiet_output;
extern */

void printusage()
{
		printf("gdsto3d  version %s\n", GDS23DS_VERSION);
		printf("Copyright (C) 2004,2005 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
		printf("gdsto3d comes with ABSOLUTELY NO WARRANTY.  You may distribute gdsto3d freely\nas described in the readme.txt distributed with this file.\n\n");
		printf("gdsto3d is a program for converting a GDS2 file to a number of 3D formats.\n\n");
		printf("Usage: gdsto3d input.gds output.pov [-c config.txt] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
		printf("Options\n");
		printf(" -c\t\tSpecify config file\n");
		printf(" -p\t\tSpecify process file\n");
		printf(" -q\t\tQuiet output\n");
		printf(" -t\t\tSpecify top cell name\n");
		printf(" -v\t\tVerbose output\n\n");
		printf("See http://www.atchoo.org/gdsto3d/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output = 1;

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
			if(strncmp(argv[i], "-c", strlen("-c"))==0){

				if(i==argc-1){
					printf("Error: -c switch given but no config file specified.\n\n");
					printusage();
					return 1;
				}else{
					configfile = argv[i+1];
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

	class GDSParse *Parser = new class GDSParse(gdsfile, povfile, configfile, processfile, topcell);
	delete Parser;
	return 0;
}

