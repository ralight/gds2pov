#include <stdio.h>
#include <stdlib.h>

#include "gds2pov.h"
#include "gdsparse.h"
#include "gds_globals.h"
#include "process_cfg.h"


int main(int argc, char *argv[])
{
	if(argc!=4){
		printf("Usage: gds2pov infile.gds outfile.pov process.txt\n");
		return 0;
	}

	class GDSParse *Parser = new class GDSParse(argv[1], argv[2], argv[3]);
	delete Parser;
	return 0;
}

