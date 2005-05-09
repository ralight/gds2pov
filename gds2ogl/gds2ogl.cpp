#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "gds2ogl.h"
#include "gdsparse.h"
#include "gds_globals.h"
#include "process_cfg.h"

#define GDS2OGL_VERSION "0.8"

/*extern int verbose_output;
extern bool quiet_output;
extern */

void printusage()
{
		printf("gds2ogl  version %s\n", GDS2OGL_VERSION);
		printf("Copyright (C) 2004,2005 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
		printf("gdsto3d comes with ABSOLUTELY NO WARRANTY.  You may distribute gdsto3d freely\nas described in the readme.txt distributed with this file.\n\n");
		printf("gdsto3d is a program for converting a GDS2 file to a number of 3D formats.\n\n");
		printf("Usage: gds2ogl input.gds output.c [-c config.txt] [-d] [-m solid|wire] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
		printf("Options\n");
		printf(" -c\t\tSpecify config file\n");
		printf(" -d\t\tDecompose polygons into triangles when using pov output format\n");
		printf(" -m\t\tSpecify solid or wireframe mode. Defaults to solid if not specified\n");
		printf(" -p\t\tSpecify process file\n -q\t\tQuiet output\n -t\t\tSpecify top cell name\n");
		printf(" -v\t\tVerbose output\n\n");
		printf("See http://www.atchoo.org/gdsto3d/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output = 1;
	decompose = false;
	render_mode = GL_FILL;

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
			}else if(strncmp(argv[i], "-d", strlen("-d"))==0){
				decompose = true;
			}else if(strncmp(argv[i], "-m", strlen("-m"))==0){
				if(i==argc-1){
					printf("Error: -m switch given but no render mode specified.\n\n");
					printusage();
					return 1;
				}else{
					if(strncmp(argv[i+1], "solid", strlen("solid"))==0){
						render_mode = GL_FILL;
					}else if(strncmp(argv[i+1], "wire", strlen("wire"))==0){
						render_mode = GL_LINE;
					}else{
						printf("Error: Invalid render mode.\n\n");
						printusage();
						return 1;
					}
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

