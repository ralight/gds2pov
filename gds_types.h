#ifndef _GDS_TYPES_H
#define _GDS_TYPES_H

short GetBitArray(FILE *fptr, short *length);
double GetEightByteReal(FILE *fptr, short *length);
long GetFourByteSignedInt(FILE *fptr, short *length);
short GetTwoByteSignedInt(FILE *fptr, short *length);
char *GetAsciiString(FILE *fptr, short *length);

#endif
