#include <stdio.h>
#include <stdlib.h>

#include "gds2pov.h"
#include "gdsparse.h"
#include "gds_globals.h"
#include "process_cfg.h"

#define VERSION 0.5

void printusage()
{
		printf("gds2pov  version %.1f\n", VERSION);
		printf("Copyright (C) 2004 by Roger Light\nhttp://www.atchoo.org/gds2pov/\n\n");
		printf("gds2pov comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2pov freely as described in the readme.txt distributed with this file.\n\n");
		printf("gds2pov is a program for converting a GDS2 file to a POV-Ray scene file.\n\n");
		printf("Usage: gds2pov input.gds output.pov [-c config.txt] [-p process.txt]\n\n");
		printf("Options\n -c\t\tSpecify config file\n -p\t\tSpecify process file\n\n");
		printf("See http://www.atchoo.org/gds2pov/ for updates.\n");
}

int main(int argc, char *argv[])
{
	if(argc<3 || argc==4 || argc==6 || argc>7){
		printusage();
		return 0;
	}

	char *gdsfile=argv[1];
	char *povfile=argv[2];

	char *configfile=NULL;
	char *processfile=NULL;

	if(argc>4){
		if(strncmp(argv[3], "-c",strlen("-c"))!=0 && strncmp(argv[3], "-p",strlen("-p"))!=0){
			printusage();
			return 0;
		}else{
			if(strncmp(argv[3], "-c", strlen("-c"))==0){
				configfile = argv[4];
			}else if(strncmp(argv[3], "-p", strlen("-p"))==0){
				processfile = argv[4];
			}else{
				printusage();
			}

		}
	}
	if(argc>6){
		if( (strncmp(argv[5], "-c",strlen("-c"))!=0 && strncmp(argv[5], "-p",strlen("-p"))!=0) || strncmp(argv[3], argv[5], strlen(argv[3]))==0){
			printusage();
			return 0;
		}else{
			if(strncmp(argv[5], "-c", strlen("-c"))==0){
				configfile = argv[6];
			}else if(strncmp(argv[5], "-p", strlen("-p"))==0){
				processfile = argv[6];
			}else{
				printusage();
			}

		}
	}

	class GDSParse *Parser = new class GDSParse(gdsfile, povfile, configfile, processfile);
	delete Parser;
	return 0;
}

