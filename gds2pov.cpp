#include <stdio.h>
#include <stdlib.h>

#include "gds2pov.h"
#include "gdsparse.h"
#include "gds_globals.h"
#include "process_cfg.h"

#define VERSION 0.7

extern bool verbose_output;

void printusage()
{
		printf("gds2pov  version %.1f\n", VERSION);
		printf("Copyright (C) 2004 by Roger Light\nhttp://www.atchoo.org/gds2pov/\n\n");
		printf("gds2pov comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2pov freely as described in the readme.txt distributed with this file.\n\n");
		printf("gds2pov is a program for converting a GDS2 file to a POV-Ray scene file.\n\n");
		printf("Usage: gds2pov input.gds output.pov [-c config.txt] [-p process.txt] [-t topcell] [-v]\n\n");
		printf("Options\n -c\t\tSpecify config file\n -p\t\tSpecify process file\n -t\t\tSpecify top cell name\n -v\t\tVerbose output\n\n");
		printf("See http://www.atchoo.org/gds2pov/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output=false;

	if(argc<3 || argc>8){
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
					printusage();
					return 1;
				}else{
					configfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-p", strlen("-p"))==0){
				if(i==argc-1){
					printusage();
					return 1;
				}else{
					processfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-t", strlen("-t"))==0){
				if(i==argc-1){
					printusage();
					return 1;
				}else{
					topcell = argv[i+1];
				}
			}else if(strncmp(argv[i], "-v", strlen("-v"))==0){
				verbose_output = true;
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

