/*
 * File: gds2svg
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the main body of the gds2svg program.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <cstdio>
#include <cstring>
#include <string>

#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsparse_svg.h"

int verbose_output;

void printusage()
{
	printf("gds2svg  version %s\n", VERSION);
	printf("Copyright (C) 2005-2008 by Roger Light\nhttp://atchoo.org/gds2pov/\n\n");
	printf("gds2svg comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2svg freely\nas described in the readme.txt distributed with this file.\n\n");
	printf("gds2svg is a program for converting a GDS2 file to an SVG file.\n\n");
	printf("Usage: gds2svg [-b] [-d] [-g] [-h] [-i input.gds] [-o output.svg] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
	printf("Options\n");
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
			if(strncmp(argv[i], "-g", strlen("-g"))==0){
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

	class GDSProcess *process=NULL;
	/*
	** Order of precedence for process.txt:
	** -p switch (given as an argument to this function)
	** Use process.txt if none others specified.
	*/
	if(processfile == ""){
		processfile = "process.txt";
	}
	process = new GDSProcess();

	if(!process){
		fprintf(stderr, "Error: Out of memory.\n");
		return -1;
	}

	if(!generate_process){
		process->Parse(processfile);
		if(!process->IsValid()){
			fprintf(stderr, "Error: %s is not a valid process file\n", processfile.c_str());
			delete process;
			return -1;
		}else if(process->LayerCount()==0){
			fprintf(stderr, "Error: No layers found in \"%s\".\n", processfile.c_str());
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
	FILE *optr;
	if(svgfile != ""){
		optr = fopen(svgfile.c_str(), "wt");
	}else{
		optr = stdout;
	}
	if(!optr){
		fprintf(stderr, "Error: Unable to open %s.\n", svgfile.c_str());
		if(iptr != stdin){
			fclose(iptr);
		}
		delete process;
		return -1;
	}

	if(iptr){
		class GDSParse_svg *Parser = new class GDSParse_svg(process, optr, generate_process);
		if(!Parser->Parse(iptr)){
			if(!generate_process){
				Parser->Output(topcell);
			}else{
				process->Save(processfile);
			}
		}

		if(iptr != stdin){
			fclose(iptr);
		}
		if(optr != stdout){
			fclose(optr);
		}

		delete Parser;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile.c_str());
		if(optr != stdout){
			fclose(optr);
		}
		delete process;
		return -1;
	}
	return 0;
}
