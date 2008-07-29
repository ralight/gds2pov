/*
 * File: gdstext.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSText class which is used to represent the GDS text object.
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

#include <cstring>

#include "gdsobject.h"
#include "gdstext.h"

GDSText::GDSText(float X, float Y, float Z, bool Flipped, float Mag, int VJust, int HJust, struct ProcessLayer *Layer) :
	_X(X), _Y(Y), _Z(Z), _Flipped(Flipped), _Mag(Mag), _VJust(VJust),
	_HJust(HJust), _Layer(Layer), _String("")
{
}

GDSText::~GDSText()
{
}

void GDSText::SetString(std::string String)
{
	_String = String;
}

void GDSText::SetRotation(float X, float Y, float Z)
{
	_Rotate.X = X;
	_Rotate.Y = Y;
	_Rotate.Z = Z;
}

std::string GDSText::GetString()
{
	return _String;
}

float GDSText::GetX()
{
	return _X;
}

float GDSText::GetY()
{
	return _Y;
}

float GDSText::GetZ()
{
	return _Z;
}

float GDSText::GetRY()
{
	return _Rotate.Y;
}

float GDSText::GetMag()
{
	return _Mag;
}

int GDSText::GetVJust()
{
	return _VJust;
}

int GDSText::GetHJust()
{
	return _HJust;
}

bool GDSText::GetFlipped()
{
	return _Flipped;
}

struct ProcessLayer *GDSText::GetLayer()
{
	return _Layer;
}

