/*
 * File: gdselements.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef _GDSELEMENTS_H_
#define _GDSELEMENTS_H_

#include <string>

typedef struct {
	float X;
	float Y;
	float Angle;
} Point;

class Transform {
public:
	float X;
	float Y;
	float Z;
	Transform() : X(0.0), Y(0.0), Z(0.0) {};
};

typedef struct {
	float R;
	float G;
	float B;
	float F;
	int Metal;
} ElementColour;

class ASRefElement {
public:
	float X1;
	float Y1;
	float X2;
	float Y2;
	float X3;
	float Y3;
	float Mag;
	int Columns;
	int Rows;
	std::string Name;
	bool Flipped;
	class GDSObject *object;
	Transform Rotate;
	
	ASRefElement() : X1(0.0), Y1(0.0), X2(0.0), Y2(0.0),
			X3(0.0), Y3(0.0), Mag(1.0), Columns(0),
			Rows(0), Name(""), Flipped(false), object(NULL) {};
};

struct _Boundary {
	float XMin;
	float XMax;
	float YMin;
	float YMax;
};

#endif
