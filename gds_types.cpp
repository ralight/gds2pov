#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

