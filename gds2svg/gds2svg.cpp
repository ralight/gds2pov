/*
 * File: gds2svg
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the main body of the gds2svg program.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <cstdio>
#include <cstring>
#include <string>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsparse_svg.h"

extern int verbose_output;

void printusage()
{
	printf("gds2svg  version %s\n", VERSION);
	printf("Copyright (C) 2005-2008 by Roger Light\nhttp://atchoo.org/gds2pov/\n\n");
	printf("gds2svg comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2svg freely\nas described in the readme.txt distributed with this file.\n\n");
	printf("gds2svg is a program for converting a GDS2 file to an SVG file.\n\n");
	printf("Usage: gds2svg [-b] [-c config.txt] [-d] [-g] [-h] [-i input.gds] [-o output.svg] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
	printf("Options\n");
	printf(" -c\t\tSpecify config file\n");
	//printf(" -d\t\tDecompose polygons into triangles (use mesh2 object instead of prism)\n");
	printf(" -g\t\tGenerate a process file based on the input gds2 file (suppresses SVG file generation).\n");
	printf(" -h\t\tDisplay this help\n");
	printf(" -i\t\tInput GDS2 file (stdin if not specified)\n");
	printf(" -o\t\tOutput SVG file (stdout if not specified)\n");
	printf(" -p\t\tSpecify process file\n");
	printf(" -q\t\tQuiet output\n");
	printf(" -t\t\tSpecify top cell name\n");
	printf(" -v\t\tVerbose output\n\n");
	printf("See http://atchoo.org/gds2pov/ for updates.\n");
}

int main(int argc, char *argv[])
{
	std::string gdsfile="";
	std::string svgfile="";
	std::string configfile="";
	std::string processfile="";
	std::string topcell="";

	bool generate_process = false;

	verbose_output = 1;

	if(argc>17){
		fprintf(stderr, "Error: Invalid number of arguments.\n\n");
		printusage();
		return 1;
	}

	for(int i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-c", strlen("-c"))==0){

				if(i==argc-1){
					printf("Error: -c switch given but no config file specified.\n\n");
					printusage();
					return 1;
				}else{
					configfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-g", strlen("-g"))==0){
				generate_process = true;
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
					svgfile = argv[i+1];
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

	if(configfile != ""){
		config = new GDSConfig(configfile);
	}else{
		config = new GDSConfig(); // Start with default positions
	}
	if(!config){
		fprintf(stderr, "Error: Out of memory.\n");
	}else if(!config->IsValid()){
		fprintf(stderr, "Error: %s is not a valid config file.\n", configfile.c_str());
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
	if(processfile == ""){
		if(config->GetProcessFile()!=""){
			processfile = config->GetProcessFile();
		}else{
			processfile = "process.txt";
		}
	}
	process = new GDSProcess();

	if(!process){
		fprintf(stderr, "Error: Out of memory.\n");
		delete config;
		return -1;
	}

	if(!generate_process){
		process->Parse(processfile);
		if(!process->IsValid()){
			fprintf(stderr, "Error: %s is not a valid process file\n", processfile.c_str());
			delete config;
			delete process;
			return -1;
		}else if(process->LayerCount()==0){
			fprintf(stderr, "Error: No layers found in \"%s\".\n", processfile.c_str());
			delete config;
			delete process;
			return -1;
		}
	}else{
		
	}

	FILE *iptr;
	if(gdsfile != ""){
		iptr = fopen(gdsfile.c_str(), "rb");
	}else{
		iptr = stdin;
	}
	if(iptr){
		class GDSParse_svg *Parser = new class GDSParse_svg(config, process, generate_process);
		if(!Parser->Parse(iptr)){
			if(!generate_process){
				FILE *optr;
				if(svgfile != ""){
					optr = fopen(svgfile.c_str(), "wt");
				}else{
					optr = stdout;
				}

				if(optr){
					Parser->Output(optr, topcell);
					if(optr != stdout){
						fclose(optr);
					}
				}else{
					fprintf(stderr, "Error: Unable to open %s.\n", svgfile.c_str());
				}
			}else{
				process->Save(processfile);
			}
		}

		if(iptr != stdin){
			fclose(iptr);
		}

		delete Parser;
		delete config;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile.c_str());
		delete config;
		delete process;
		return -1;
	}
	return 0;
}

