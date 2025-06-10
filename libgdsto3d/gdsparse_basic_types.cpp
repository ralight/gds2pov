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


short GDSParse::GetBitArray()
{
	uint8_t byte1;

	fread(&byte1, 1, 1, m_iptr);
	fread(&byte1, 1, 1, m_iptr);

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

	fread(&value, 1, 1, m_iptr);
	if(value & 128){
		value -= 128;
		sign = -1.0;
	}
	exponent = (double )value;
	exponent -= 64.0;
	mant=0.0;

	fread(&b2, 1, 1, m_iptr);
	fread(&b3, 1, 1, m_iptr);
	fread(&b4, 1, 1, m_iptr);
	fread(&b5, 1, 1, m_iptr);
	fread(&b6, 1, 1, m_iptr);
	fread(&b7, 1, 1, m_iptr);
	fread(&b8, 1, 1, m_iptr);

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
	fread(&value, 4, 1, m_iptr);

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

	fread(&value, 2, 1, m_iptr);

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
			return NULL;
		}
		fread(str, 1, m_recordlen, m_iptr);
		str[m_recordlen] = 0;
		m_recordlen = 0;
	}
	return str;
}
