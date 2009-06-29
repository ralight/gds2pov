/*
 * File: gdspath.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSPath class which is used to represent the GDS path type.
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

#include "gdsobject.h"
#include "gdspath.h"

GDSPath::GDSPath(int type, float height, float thickness,
				unsigned int points, float width,
				float bgnextn, float endextn,
				class ProcessLayer *layer) :
				m_type(type), m_height(height), m_thickness(thickness), m_points(points),
				m_width(width), m_bgnextn(bgnextn), m_endextn(endextn), m_layer(layer)
{
	m_coords = new Point[points];
}

GDSPath::~GDSPath()
{
	if(m_coords) delete [] m_coords;
}

void GDSPath::AddPoint(unsigned int index, float x, float y)
{
	if(index < m_points){
		m_coords[index].x = x;
		m_coords[index].y = y;
	}
}


void GDSPath::SetRotation(float x, float y, float z)
{
	m_rotate.x = x;
	m_rotate.y = y;
	m_rotate.z = z;
}

float GDSPath::GetXCoords(unsigned int index)
{
	if(index < m_points){
		return m_coords[index].x;
	}
	return 0.0;
}

float GDSPath::GetYCoords(unsigned int index)
{
	if(index < m_points){
		return m_coords[index].y;
	}
	return 0.0;
}

unsigned int GDSPath::GetPoints()
{
	return m_points;
}

float GDSPath::GetHeight()
{
	return m_height;
}

float GDSPath::GetThickness()
{
	return m_thickness;
}

float GDSPath::GetWidth()
{
	return m_width;
}

float GDSPath::GetBgnExtn()
{
	return m_bgnextn;
}

float GDSPath::GetEndExtn()
{
	return m_endextn;
}

int GDSPath::GetType()
{
	return m_type;
}

class ProcessLayer *GDSPath::GetLayer()
{
	return m_layer;
}

