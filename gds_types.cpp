#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "gds2pov.h"

/* Win32 specific byte swapping code */

long endian_swap_long(long value)
{
	_asm{
		mov eax,value
		bswap eax
		mov value,eax
	}
	return value;
}

short endian_swap_short(short value)
{
	_asm{
		mov ax,value
		xchg ah,al
		mov value,ax
	}
	return value;
}

