/*
 * File: gdsto3d.h
 * Author: Roger Light
 * Project: gdsto3d
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

#ifndef GDSTO3D_H
#define GDSTO3D_H

#include <string>

struct GDSVertex{
	float x, y, z;
};

struct GDSTriangle{
	int v[3];
};

class Point {
public:
	float x;
	float y;
	float angle;
	Point() : x(0.0), y(0.0), angle(0.0) {};
};

class Transform {
public:
	float x;
	float y;
	float z;
	Transform() : x(0.0), y(0.0), z(0.0) {};
};

class ASRefElement {
public:
	float x1;
	float y1;
	float x2;
	float y2;
	float x3;
	float y3;
	float mag;
	int columns;
	int rows;
	std::string name;
	bool flipped;
	class GDSObject *object;
	Transform rotate;

	ASRefElement() : x1(0.0), y1(0.0), x2(0.0), y2(0.0),
			x3(0.0), y3(0.0), mag(1.0), columns(0),
			rows(0), name(""), flipped(false), object(NULL) {};
};

struct _Boundary {
	float xmin;
	float xmax;
	float ymin;
	float ymax;
};

#endif
