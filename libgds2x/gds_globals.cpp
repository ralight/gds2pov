/*
 * File: gds_globals.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This file contains non-class functions.
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

#include <cstdio>
#include <stdarg.h>

#include "gds_globals.h"

namespace GDS2X {

int verbose_output;

void v_printf(const int level, const char *fmt, ...)
{
	if(verbose_output>=level){
		va_list va;
		va_start(va, fmt);
		vprintf(fmt, va);
		va_end(va);
	}
}

void SetVerbosity(int level)
{
	verbose_output = level;
}

Vertex CreateVertex(float x, float y, float z)
{
	Vertex result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}


Triangle CreateTriangle(int v1, int v2, int v3)
{
	Triangle result;
	result.v[0] = v1;
	result.v[1] = v2;
	result.v[2] = v3;
	return result;
}

};
