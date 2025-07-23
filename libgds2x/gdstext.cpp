/*
 * File: gdstext.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the Text class which is used to represent the GDS text object.
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

#include <cstring>

#include "gdsobject.h"
#include "gdstext.h"

namespace GDS2X {

Text::Text(double x, double y, double z,
				bool flipped, double mag,
				int vjust, int hjust,
				ProcessLayer *layer) :
				m_x(x), m_y(y), m_z(z),
				m_flipped(flipped), m_mag(mag),
				m_vjust(vjust), m_hjust(hjust),
				m_layer(layer), m_string("")
{
}

Text::~Text()
{
}

void Text::SetString(std::string String)
{
	m_string = String;
}

void Text::SetRotation(double x, double y, double z)
{
	m_rotate.x = x;
	m_rotate.y = y;
	m_rotate.z = z;
}

std::string Text::GetString()
{
	return m_string;
}

double Text::GetX()
{
	return m_x;
}

double Text::GetY()
{
	return m_y;
}

double Text::GetZ()
{
	return m_z;
}

double Text::GetRY()
{
	return m_rotate.y;
}

double Text::GetMag()
{
	return m_mag;
}

int Text::GetVJust()
{
	return m_vjust;
}

int Text::GetHJust()
{
	return m_hjust;
}

bool Text::GetFlipped()
{
	return m_flipped;
}

ProcessLayer *Text::GetLayer()
{
	return m_layer;
}

}
