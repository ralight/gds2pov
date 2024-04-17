/*
 * File: gdspolygon.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSPolyon class which is used to represent the GDS boundary
 * object.
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

#include "gdsobject.h"
#include "gdspolygon.h"

GDSPolygon::GDSPolygon(float height, float thickness, unsigned int points, class ProcessLayer *layer) :
	m_height(height), m_thickness(thickness), m_points(points), m_layer(layer)
{
	m_coords = new Point[points+1]; //FIXME - debug +1
}

GDSPolygon::~GDSPolygon()
{
	if(m_coords) delete [] m_coords;
}

void GDSPolygon::AddPoint(unsigned int index, float x, float y)
{
	if(index < m_points){
		m_coords[index].x = x;
		m_coords[index].y = y;
	}
}


void GDSPolygon::SetRotation(float x, float y, float z)
{
	m_rotate.x = x;
	m_rotate.y = y;
	m_rotate.z = z;
}

float GDSPolygon::GetXCoords(unsigned int index)
{
	// FIXME - should probably use exceptions here
	if(index < m_points){
		return m_coords[index].x;
	}
	return 0.0; 
}

float GDSPolygon::GetYCoords(unsigned int index)
{
	// FIXME - should probably use exceptions here
	if(index < m_points){
		return m_coords[index].y;
	}
	return 0.0; 
}

float GDSPolygon::GetAngleCoords(unsigned int index)
{
	// FIXME - should probably use exceptions here
	if(index < m_points){
	return m_coords[index].angle;
	}
	return 0.0; 
}

void GDSPolygon::SetAngleCoords(unsigned int index, float value)
{
	// FIXME - should probably use exceptions here
	if(index < m_points){
		m_coords[index].angle = value;
	}
}

unsigned int GDSPolygon::GetPoints(void)
{
	return m_points;
}

float GDSPolygon::GetHeight(void)
{
	return m_height;
}

float GDSPolygon::GetThickness(void)
{
	return m_thickness;
}

class ProcessLayer *GDSPolygon::GetLayer(void)
{
	return m_layer;
}

