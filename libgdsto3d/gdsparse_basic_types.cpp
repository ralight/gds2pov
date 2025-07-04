/*
 * File: gdsparse.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSParse class which is used to parse a GDS file and create a
 * GDSObjects object containing the contents of the file according to the
 * process configuration.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include <cmath>
#include <cstring>

#include "gdsparse.h"

/*
 * We need byte swapping functions.
 * If these exist for a particular platform, use the (presumably) optimised
 * versions provided. Otherwise, use the network functions htonl() and htons().
 */

#if defined(linux)
#	include <endian.h>
#	include <byteswap.h>
#	define endian_swap_long(A) bswap_32((A))
#	define endian_swap_short(A) bswap_16((A))
#elif defined(WIN32)
//#	define __LITTLE_ENDIAN 1234
//#	define __BIG_ENDIAN 4321
//#	define __BYTE_ORDER __LITTLE_ENDIAN
//	long endian_swap_long(long value);
//	short endian_swap_short(short value);
#   define byte win_byte_override
#	include <winsock2.h>
#   undef byte
#	define endian_swap_long(A) htonl((A))
#	define endian_swap_short(A) htons((A))
#else
	/* Generic includes for Unix-alike systems */
	/* Includes at least Solaris, FreeBSD, HP-UX */
#	include <arpa/nameser_compat.h>
#	include <netinet/in.h>
#	define endian_swap_long(A) htonl((A))
#	define endian_swap_short(A) htons((A))
#endif


short GDSParse::GetBitArray()
{
	uint8_t byte1;

	if(fread(&byte1, 1, 1, m_iptr) != 1
			|| fread(&byte1, 1, 1, m_iptr) != 1){
		return 0;
	}

	m_recordlen-=2;
	return 0;
}

double GDSParse::GetEightByteReal()
{
	uint8_t value;
	uint8_t b8, b2, b3, b4, b5, b6, b7;
	double sign=1.0;
	double exponent;
	double mant;

	if(fread(&value, 1, 1, m_iptr) != 1){
		return 0.0;
	}
	if(value & 128){
		value -= 128;
		sign = -1.0;
	}
	exponent = (double )value;
	exponent -= 64.0;
	mant=0.0;

	if(fread(&b2, 1, 1, m_iptr) != 1
			|| fread(&b3, 1, 1, m_iptr) != 1
			|| fread(&b4, 1, 1, m_iptr) != 1
			|| fread(&b5, 1, 1, m_iptr) != 1
			|| fread(&b6, 1, 1, m_iptr) != 1
			|| fread(&b7, 1, 1, m_iptr) != 1
			|| fread(&b8, 1, 1, m_iptr) != 1
			){
		return 0.0;
	}

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

	m_recordlen-=8;

	return sign*(mant*pow(16.0,exponent));
}

int32_t GDSParse::GetFourByteSignedInt()
{
	int32_t value;
	if(fread(&value, 4, 1, m_iptr) != 1){
		return 0;
	}

	m_recordlen-=4;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	return endian_swap_long(value);
#else
	return value;
#endif
}

int16_t GDSParse::GetTwoByteSignedInt()
{
	int16_t value;

	if(fread(&value, 2, 1, m_iptr) != 1){
		return 0;
	}

	m_recordlen-=2;

#if __BYTE_ORDER == __LITTLE_ENDIAN
	return endian_swap_short(value);
#else
	return value;
#endif
}

char *GDSParse::GetAsciiString()
{
	char *str=NULL;

	if(m_recordlen>0){
		m_recordlen += m_recordlen%2; /* Make sure length is even */
		str = new char[m_recordlen+1];
		if(!str){
			fprintf(stderr, "Unable to allocate memory for ascii string (%d)\n", m_recordlen);
			return nullptr;
		}
		if(fread(str, 1, m_recordlen, m_iptr) != (size_t)m_recordlen){
			return nullptr;
		}
		str[m_recordlen] = 0;
		m_recordlen = 0;
	}
	return str;
}
