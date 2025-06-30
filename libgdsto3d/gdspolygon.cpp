/*
 * File: gdspolygon.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the Polyon class which is used to represent the GDS boundary
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
#include "PolygonTriangulator.h"

namespace GDS2X {

Polygon::Polygon(float height, float thickness, unsigned int points, class ProcessLayer *layer) :
	m_height(height), m_thickness(thickness), m_points(points), m_layer(layer)
{
	m_coords = new Point[points+1]; //FIXME - debug +1
}

Polygon::~Polygon()
{
	if(m_coords) delete [] m_coords;
}

void Polygon::AddPoint(unsigned int index, float x, float y)
{
	if(index < m_points){
		m_coords[index].x = x;
		m_coords[index].y = y;
	}
}


void Polygon::SetRotation(float x, float y, float z)
{
	m_rotate.x = x;
	m_rotate.y = y;
	m_rotate.z = z;
}

float Polygon::GetXCoords(unsigned int index)
{
	if(index < m_points){
		return m_coords[index].x;
	}
	return 0.0;
}

float Polygon::GetYCoords(unsigned int index)
{
	if(index < m_points){
		return m_coords[index].y;
	}
	return 0.0;
}

float Polygon::GetAngleCoords(unsigned int index)
{
	if(index < m_points){
	return m_coords[index].angle;
	}
	return 0.0;
}

void Polygon::SetAngleCoords(unsigned int index, float value)
{
	if(index < m_points){
		m_coords[index].angle = value;
	}
}

unsigned int Polygon::GetPoints(void)
{
	return m_points;
}

float Polygon::GetHeight(void)
{
	return m_height;
}

float Polygon::GetThickness(void)
{
	return m_thickness;
}

class ProcessLayer *Polygon::GetLayer(void)
{
	return m_layer;
}


static Vertex CreateVertex(float x, float y, float z)
{
	Vertex result;
	result.x = x;
	result.y = y;
	result.z = z;
	return result;
}


static Triangle CreateTriangle(int v1, int v2, int v3)
{
	Triangle result;
	result.v[0] = v1;
	result.v[1] = v2;
	result.v[2] = v3;
	return result;
}


std::vector<Vertex> Polygon::GetVertices()
{
	std::vector<Vertex> vertices;

	for(unsigned int j=0; j<this->GetPoints()-1; j++){
		vertices.push_back(CreateVertex(
			this->GetXCoords(j),
			this->GetYCoords(j),
			this->GetHeight() + this->GetThickness()
		));
	}
	for(unsigned int j=0; j<this->GetPoints()-1; j++){
		vertices.push_back(CreateVertex(
			this->GetXCoords(j),
			this->GetYCoords(j),
			this->GetHeight()
		));
	}

	return vertices;
}

std::vector<Triangle> Polygon::GetTriangles()
{
	/* Vertical faces */
	unsigned int j=0;
	unsigned int count = this->GetPoints()-1;

	std::vector<Triangle> triangles;

	for(j=0; j<count; j++){
		triangles.push_back(CreateTriangle(j, j+count-1, j+count));
		triangles.push_back(CreateTriangle(j+count, j+1, j));
	}

	/* Horizontal faces */
	std::vector<double> x_coords;
	std::vector<double> y_coords;

	for(unsigned int i=0; i<this->GetPoints()-1; i++){
		x_coords.insert(x_coords.begin(), this->GetXCoords(i));
		y_coords.insert(y_coords.begin(), this->GetYCoords(i));
	}

	auto triangulation = PolygonTriangulator(x_coords, y_coords);
	auto pg_triangles = triangulation.triangles();

	for(auto vec = pg_triangles->begin(); vec != pg_triangles->end(); vec++){
		triangles.push_back(CreateTriangle(1*count-vec->at(2), 1*count-vec->at(1), 1*count-vec->at(0)));
		triangles.push_back(CreateTriangle(2*count-vec->at(0), 2*count-vec->at(1), 2*count-vec->at(2)));
	}

	return triangles;
}

}
