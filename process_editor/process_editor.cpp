/*
 * File: gds2pov
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the main body of the gds2pov program.
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
#include "gdsparse_pov.h"

int main(int argc, char *argv[])
{
	std::string gdsfile="";
	std::string processfile="";
	bool generate_process = true;

	gdsfile = argv[1];

	class GDSConfig *config = new GDSConfig();

	/************ Load process ****************/

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

	process->Parse(processfile);

	/************ Open GDS2 file and parse ****************/

	FILE *iptr;
	iptr = fopen(gdsfile.c_str(), "rb");
	if(iptr){
		class GDSParse *Parser = new class GDSParse(config, process, generate_process);
		if(!Parser->Parse(iptr)){

			ProcessLayer *layer;

			for(unsigned int i = 0; i < process->LayerCount(); i++){
				layer = process->GetLayer(i);
				printf("%s - %d:%d\n", layer->Name.c_str(), layer->Layer, layer->Datatype);
			}
/*
			if(!generate_process){

			}else{
				process->Save(processfile);
			}
*/
		}

		fclose(iptr);

		delete Parser;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile.c_str());
		delete process;
		return -1;
	}
	return 0;
}

