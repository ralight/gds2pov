#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "gds2pov.h"
#include "gdsparse.h"
#include "gds_globals.h"
#include "process_cfg.h"

#define VERSION "0.8"

/*extern int verbose_output;
extern bool quiet_output;
extern */

void printusage()
{
		printf("gdsto3d  version %s\n", VERSION);
		printf("Copyright (C) 2004,2005 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
		printf("gdsto3d comes with ABSOLUTELY NO WARRANTY.  You may distribute gdsto3d freely\nas described in the readme.txt distributed with this file.\n\n");
		printf("gdsto3d is a program for converting a GDS2 file to a number of 3D formats.\n\n");
		printf("Usage: gdsto3d input.gds output.pov [-b] [-c config.txt] [-d] [-f pov|3ds|ogl] [-m solid|wire] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
		printf("Options\n");
		printf(" -b\t\tOutput bounding box instead of layout to allow easier and\n\t\tquicker placing of the camera\n");
		printf(" -c\t\tSpecify config file\n");
		printf(" -d\t\tDecompose polygons into triangles when using pov output format\n");
		printf(" -f\t\tSpecify output file format from 3ds and pov. Default is pov\n\t\tif not specified\n");
		printf(" -m\t\tFor OpenGL output, specify solid or wireframe mode\n");
		printf(" -p\t\tSpecify process file\n -q\t\tQuiet output\n -t\t\tSpecify top cell name\n");
		printf(" -v\t\tVerbose output\n\n");
		printf("See http://www.atchoo.org/gdsto3d/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output = 1;
	bounding_output = false;
	output_format = opPOV;
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
			}else if(strncmp(argv[i], "-f", strlen("-f"))==0){
				if(i==argc-1){
					printf("Error: -f switch given but no output format specified.\n\n");
					printusage();
					return 1;
				}else{
					if(strncmp(argv[i+1], "pov", strlen("pov"))==0){
						output_format = opPOV;
					}else if(strncmp(argv[i+1], "3ds", strlen("3ds"))==0){
						output_format = op3DS;
					}else if(strncmp(argv[i+1], "ogl", strlen("ogl"))==0){
						output_format = opOGL;
					}else{
						printf("Error: Invalid output format.\n\n");
						printusage();
						return 1;
					}
				}
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

