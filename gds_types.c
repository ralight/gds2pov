#include <stdio.h>
#include <stdlib.h>
//#include <byteswap.h>
//#include <endian.h>
#include <string.h>
#include <math.h>

#ifdef LINUX
#include <byteswap.h>
#endif

#include "gds2pov.h"

long endian_swap_long(long value)
{
#ifdef WIN32
	_asm{
		mov eax,value
		bswap eax
		mov value,eax
	}
#endif
#ifdef LINUX
	value = bswap_32(value);
#endif
	return value;
}

short endian_swap_short(short value)
{
#ifdef WIN32
	_asm{
		mov ax,value
		xchg ah,al
		mov value,ax
	}
#endif
#ifdef LINUX
	value = bswap_16(value);
#endif
	return value;
}

short GetBitArray(FILE *fptr, short *length)
{
	byte byte1;

	fread(&byte1, 1, 1, fptr);
	fread(&byte1, 1, 1, fptr);

	if(length){
		(*length)-=2;
	}
	return 0;
}


double GetEightByteReal(FILE *fptr, short *length)
{
	byte value;
	byte b8, b2, b3, b4, b5, b6, b7;
	double sign=1.0;
	double exponent;
	double mant;

	fread(&value, 1, 1, fptr);
	if(value & 128){
		value -= 128;
		sign = -1.0;
	}
	exponent = (double )value;
	exponent -= 64.0;
	mant=0.0;

	fread(&b2, 1, 1, fptr);
	fread(&b3, 1, 1, fptr);
	fread(&b4, 1, 1, fptr);
	fread(&b5, 1, 1, fptr);
	fread(&b6, 1, 1, fptr);
	fread(&b7, 1, 1, fptr);
	fread(&b8, 1, 1, fptr);

	mant += b8;
	mant /= 256.0;
	mant += b7;
	mant /= 256.0;
	mant += b6;
	mant /= 256.0;
	mant += b5;
	mant /= 256.0;
	mant += b4;
	mant /= 256.0;
	mant += b3;
	mant /= 256.0;
	mant += b2;
	mant /= 256.0;

	if(length){
		(*length)-=8;
	}
	return sign*(mant*pow(16.0,exponent));
}

long GetFourByteSignedInt(FILE *fptr, short *length)
{
	long value;
	fread(&value, 4, 1, fptr);
	
	if(length){
		(*length)-=4;
	}
	return endian_swap_long(value);
}

short GetTwoByteSignedInt(FILE *fptr, short *length)
{
	short value;

	fread(&value, 2, 1, fptr);

	if(length){
		(*length)-=2;
	}

	return endian_swap_short(value);
}

char *GetAsciiString(FILE *fptr, short *length)
{
	char *str=NULL;

	if(length && (*length)>0){
		(*length) += (*length)%2; /* Make sure length is even */
		str = (char *)malloc((*length)+1);
		if(!str){
			printf("Unable to allocate memory for ascii string (%d)\n", (*length));
			fclose(fptr);
			// FIXME - need to cleanup properly.
			exit(1);
		}
		fread(str, 1, (*length), fptr);
		str[(*length)] = 0;
		(*length) = 0;
	}

	return str;
}

