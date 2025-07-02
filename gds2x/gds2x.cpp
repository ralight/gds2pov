/*
 * File: gds23mf
 * Author: Roger Light
 * Project: gds2x
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
#include "gdsparse_openscad.h"
#include "gdsparse_pov.h"
#include "gdsparse_svg.h"

void printusage(std::string exe)
{
	printf("gds23mf  version %s\n", VERSION);
	printf("Copyright (C) 2004-2025 Roger Light\nhttp://atchoo.org/gds2pov/\n\n");
	printf("%s comes with ABSOLUTELY NO WARRANTY.  You may distribute %s freely\nas described in the readme.txt distributed with this file.\n\n", exe.c_str(), exe.c_str());
	printf("%s is a program for converting a GDS2 file to other graphical files.\n\n", exe.c_str());
	printf("Usage: %s [-c config.txt] [-e camera.pov] [-f 3mf|openscad|povray|stl|svg] [-h] [-i input.gds] [-o output] [-p process.txt] [-q] [-t topcell] [-v]\n\n", exe.c_str());
	printf("Options\n");
	printf(" -c\t\tSpecify scene config file (povray only)\n");
	printf(" -e\t\tSpecify external camera position file (povray only)\n");
	printf(" -f\t\tSpecify an output format from: 3mf openscad povray stl svg. Defaults to 3mf\n");
	printf(" -g\t\tGenerate a process file based on the input gds2 file (suppresses output file generation).\n");
	printf(" -h\t\tDisplay this help\n");
	printf(" -i\t\tInput GDS2 file (stdin if not specified)\n");
	printf(" -m\t\tInput macro file\n");
	printf(" -o\t\tOutput file (stdout if not specified)\n");
	printf(" -p\t\tSpecify process file\n");
	printf(" -q\t\tQuiet output\n");
	printf(" -t\t\tSpecify top cell name\n");
	printf(" -v\t\tVerbose output\n\n");
	printf("See http://atchoo.org/gds2pov/ for updates.\n");
}


GDS2X::Process *load_process(std::string processfile, bool generate_process)
{
	GDS2X::Process *process = new GDS2X::Process();
	if(!process){
		fprintf(stderr, "Error: Out of memory.\n");
		return nullptr;
	}
	if(!generate_process){
		process->Parse(processfile);
		if(!process->IsValid()){
			fprintf(stderr, "Error: %s is not a valid process file\n", processfile.c_str());
			delete process;
			return nullptr;
		}else if(process->LayerCount()==0){
			fprintf(stderr, "Error: No layers found in \"%s\".\n", processfile.c_str());
			delete process;
			return nullptr;
		}
	}
	return process;
}


int main(int argc, char *argv[])
{
	std::unordered_map<std::string, std::string> options;

	options["bounding_output"] = "false";
	options["gdsfile"] = "";
	options["generate_process"] = "false";
	options["macrofile"] = "";
	options["outfile"] = "";
	options["processfile"] = "";
	options["topcell"] = "";

	std::string exe = argv[0];
	if(exe == "gds23mf"){
		options["format"] = "3mf";
	}else if(exe == "gds2openscad"){
		options["format"] = "openscad";
	}else if(exe == "gds2pov"){
		options["format"] = "povray";
	}else if(exe == "gds2stl"){
		options["format"] = "stl";
	}else if(exe == "gds2svg"){
		options["format"] = "svg";
	}else{
		options["format"] = "3mf";
	}

	int verbose_output = 1;

	if(argc == 1){
		printusage(exe);
		return 0;
	}else if(argc > 21){
		fprintf(stderr, "Error: Invalid number of arguments.\n\n");
		printusage(exe);
		return 1;
	}

	for(int i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-b", strlen("-b"))==0){
				options["bounding_output"] = "true";
			}else if(strncmp(argv[i], "-c", strlen("-c"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -c switch given but no config file specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["configfile"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-e", strlen("-e"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -e switch given but no camera file specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["camerafile"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-f", strlen("-f"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -f switch given but no format specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["format"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-g", strlen("-g"))==0){
				options["generate_process"] = "true";
			}else if(strncmp(argv[i], "-h", strlen("-h"))==0){
				printusage(exe);
				return 0;
			}else if(strncmp(argv[i], "-i", strlen("-i"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -i switch given but no input file specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["gdsfile"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-m", strlen("-m"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -m switch given but no macro file specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["macrofile"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-o", strlen("-o"))==0){
				if(i==argc-1){
					fprintf(stderr, "Error: -o switch given but no output file specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["outfile"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-p", strlen("-p"))==0){
				if(i==argc-1){
					printf("Error: -p switch given but no process file specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["processfile"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-q", strlen("-q"))==0){
				verbose_output--;
			}else if(strncmp(argv[i], "-t", strlen("-t"))==0){
				if(i==argc-1){
					printf("Error: -t switch given but no top cell specified.\n\n");
					printusage(exe);
					return 1;
				}else{
					options["topcell"] = argv[i+1];
					i++;
				}
			}else if(strncmp(argv[i], "-v", strlen("-v"))==0){
				verbose_output++;
			}else{
				printusage(exe);
				return 1;
			}
		//}else{
		// Assume it is a process/config file specified on a previous arg
		}
	}

	GDS2X::SetVerbosity(verbose_output);

	/************ Load process ****************/

	GDS2X::Process *process = load_process(options["processfile"], options["generate_process"] == "true");
	if(!process){
		return -1;
	}

	/************ Open GDS2 file and parse ****************/

	FILE *iptr;
	if(options["gdsfile"] != ""){
		iptr = fopen(options["gdsfile"].c_str(), "rb");
	}else{
		iptr = stdin;
	}
	if(iptr){
		GDS2X::Parse *parser;

		if(options["format"] == "3mf" || options["format"] == "stl"){
			GDSParse_3mf *parser_3mf = new GDSParse_3mf(process, options);
			parser = static_cast<GDS2X::Parse *>(parser_3mf);
		}else if(options["format"] == "openscad"){
			GDSParse_openscad *parser_openscad = new GDSParse_openscad(process, options);
			parser = static_cast<GDS2X::Parse *>(parser_openscad);
		}else if(options["format"] == "povray"){
			GDSParse_pov *parser_povray = new GDSParse_pov(process, options);
			parser = static_cast<GDS2X::Parse *>(parser_povray);
		}else if(options["format"] == "svg"){
			GDSParse_svg *parser_svg = new GDSParse_svg(process, options);
			parser = static_cast<GDS2X::Parse *>(parser_svg);
		}
		if(options["macrofile"] != ""){
			parser->LoadMacroFile(options["macrofile"]);
		}
		if(!parser->ParseFile(iptr)){
			if(options["generate_process"] == "false"){
				parser->Output();
			}else{
				process->Save(options["processfile"]);
			}
		}

		if(iptr != stdin){
			fclose(iptr);
		}

		delete parser;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", options["gdsfile"].c_str());
		delete process;
		return -1;
	}
	return 0;
}
