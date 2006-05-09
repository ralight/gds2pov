/*
 * File: gds2info
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the main body of the gds2info program.
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

#include <stdio.h>
#include <string>

#include "gds_globals.h"
#include "gds2info.h"
#include "gdsparse_info.h"

extern int verbose_output;
bool decompose;

void printusage()
{
	printf("gds2info  version %s\n", VERSION);
	printf("Copyright (C) 2004-2006 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
	printf("gds2info comes with ABSOLUTELY NO WARRANTY.  You may distribute gds2info freely\nas described in the readme.txt distributed with this file.\n\n");
	printf("gds2info is a program for displaying information on a GDS2 file.\n\n");
	printf("Usage: gds2info [-h] [-i input.gds] [-q] [-t topcell] [-v]\n\n");
	printf("Options\n");
	printf(" -h\t\tDisplay this help\n");
	printf(" -i\t\tInput GDS2 file (stdin if not specified)\n");
	printf(" -q\t\tQuiet output\n");
	printf(" -t\t\tSpecify top cell name\n");
	printf(" -v\t\tVerbose output\n\n");
	printf("See http://www.atchoo.org/gds2pov/ for updates.\n");
}

int main(int argc, char *argv[])
{
	verbose_output = 1;

	if(argc>10){
		fprintf(stderr, "Error: Invalid number of arguments.\n\n");
		printusage();
		return 1;
	}

	char *gdsfile=NULL;

	char *topcell=NULL;

	for(int i=1; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-h", strlen("-h"))==0){
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
		}
	}

	FILE *iptr;
	if(gdsfile){
		iptr = fopen(gdsfile, "rb");
	}else{
		iptr = stdin;
	}
	if(iptr){
		class GDSConfig *config = new GDSConfig();
		class GDSParse_info *Parser = new class GDSParse_info(config);
		if(!Parser->Parse(iptr)){
			Parser->Output(stdout, topcell);
		}

		if(iptr != stdin){
			fclose(iptr);
		}

		delete Parser;
		delete config;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile);
		return -1;
	}
	return 0;
}

