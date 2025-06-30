/*
 * File: gds23mf
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the main body of the gds23mf program.
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

#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsparse_3mf.h"

void printusage()
{
	printf("gds23mf  version %s\n", VERSION);
	printf("Copyright (C) 2004-2008 Roger Light\nhttp://atchoo.org/gds2pov/\n\n");
	printf("gds23mf comes with ABSOLUTELY NO WARRANTY.  You may distribute gds23mf freely\nas described in the readme.txt distributed with this file.\n\n");
	printf("gds23mf is a program for converting a GDS2 file to a 3mf file.\n\n");
	printf("Usage: gds23mf [-d] [-f 3mf|stl] [-h] [-i input.gds] [-o output.3mf] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
	printf("Options\n");
	printf(" -f\t\tSpecify an output format of 3mf or stl.\n");
	printf(" -g\t\tGenerate a process file based on the input gds2 file (suppresses output file generation).\n");
	printf(" -h\t\tDisplay this help\n");
	printf(" -i\t\tInput GDS2 file (stdin if not specified)\n");
	printf(" -o\t\tOutput 3MF file (stdout if not specified)\n");
	printf(" -p\t\tSpecify process file\n");
	printf(" -q\t\tQuiet output\n");
	printf(" -t\t\tSpecify top cell name\n");
	printf(" -v\t\tVerbose output\n\n");
	printf("See http://atchoo.org/gds2pov/ for updates.\n");
}

int main(int argc, char *argv[])
{
	bool bounding_output = false;
	bool generate_process = false;

	std::string gdsfile="";
	std::string outfile="";

	std::string processfile="";
	std::string topcell="";
	std::string format="3mf";

	int verbose_output = 1;

	if(argc == 1){
		printusage();
		return 0;
	}else if(argc > 21){
		fprintf(stderr, "Error: Invalid number of arguments.\n\n");
		printusage();
		return 1;
	}

	for(int i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-b", strlen("-b"))==0){
				bounding_output = true;
			}else if(strncmp(argv[i], "-f", strlen("-f"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -f switch given but no format specified.\n\n");
					printusage();
					return 1;
				}else{
					format = argv[i+1];
					i++;
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
					outfile = argv[i+1];
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


	GDS2X::SetVerbosity(verbose_output);

	/************ Load process ****************/

	class GDS2X::Process *process=NULL;
	/*
	** Order of precedence for process.txt:
	** -p switch (given as an argument to this function)
	** Use process.txt if none others specified.
	*/
	if(processfile == ""){
		processfile = "process.txt";
	}
	process = new GDS2X::Process();
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
	}

	/************ Open GDS2 file and parse ****************/

	FILE *iptr;
	if(gdsfile != ""){
		iptr = fopen(gdsfile.c_str(), "rb");
	}else{
		iptr = stdin;
	}
	if(iptr){
		class GDSParse_3mf *parser = new class GDSParse_3mf(process, bounding_output, generate_process);
		if(!parser->ParseFile(iptr)){
			if(!generate_process){
				parser->Output(topcell, outfile, format);
			}else{
				process->Save(processfile);
			}
		}

		if(iptr != stdin){
			fclose(iptr);
		}

		delete parser;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile.c_str());
		delete process;
		return -1;
	}
	return 0;
}
