#ifndef _GDS_PARSE_H
#define _GDS_PARSE_H

#include "process_cfg.h"

void GDStoPOV(FILE *infile, FILE *outfile, layers *all_layers, int layer_count);
void Parse(FILE *infile, FILE *outfile, layers *all_layers, int layer_count);
void FindStructures(FILE *infile, FILE *outfile, layers *all_layers, int layer_count);


void ParseHeader(FILE *infile, FILE *outfile, short recordlen);
void ParseBgnLib(FILE *infile, FILE *outfile, short recordlen);
void ParseLibName(FILE *infile, FILE *outfile, short recordlen);
void ParseSName(FILE *infile, FILE *outfile, short recordlen);
void ParseUnits(FILE *infile, FILE *outfile, short recordlen);
void ParseBgnStr(FILE *infile, FILE *outfile, short recordlen);
void ParseStrName(FILE *infile, FILE *outfile, short recordlen);
void ParseBoundary(FILE *infile, FILE *outfile, short recordlen);
void ParsePath(FILE *infile, FILE *outfile, short recordlen);
void ParsePathType(FILE *infile, FILE *outfile, short recordlen);
void ParseWidth(FILE *infile, FILE *outfile, short recordlen);
void ParseLayer(FILE *infile, FILE *outfile, short recordlen);
void ParseDataType(FILE *infile, FILE *outfile, short recordlen);
void ParseXY(FILE *infile, FILE *outfile, short recordlen, layers *all_layers, int layer_count);
void ParseEndEl(FILE *infile, FILE *outfile, short recordlen);

#endif
