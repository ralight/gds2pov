/*
 * File: gdspolygon.h
 * Author: Roger Light
 * Project: gds2x
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

#include "gds_globals.h"
#include "gdsobject.h"
#include "gdspolygon.h"
#include "PolygonTriangulator.h"

namespace GDS2X {

Polygon::Polygon(float height, float thickness, unsigned int points, class ProcessLayer *layer) :
	m_height(height), m_thickness(thickness), m_points(points), m_layer(layer)
{
	m_coords = new Point[points+1]; //FIXME - debug +1
}


Polygon::Polygon(std::vector<Vertex> vertices, std::vector<Triangle> triangles, ProcessLayer *layer) :
	m_layer(layer)
{
	for(int i=0; i<vertices.size(); i++){
		m_vertices.push_back(CreateVertex(vertices[i].x, vertices[i].y, vertices[i].z));
	}
	for(int i=0; i<triangles.size(); i++){
		m_triangles.push_back(CreateTriangle(triangles[i].v[0], triangles[i].v[1], triangles[i].v[2]));
	}
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


std::vector<Vertex> Polygon::GetVertices()
{
	std::vector<Vertex> vertices;

	if(m_vertices.size() > 0){
		return m_vertices;
	}

	for(unsigned int j=0; j<this->GetPoints(); j++){
		vertices.push_back(CreateVertex(
			this->GetXCoords(j),
			this->GetYCoords(j),
			this->GetHeight() + this->GetThickness()
		));
	}
	for(unsigned int j=0; j<this->GetPoints(); j++){
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
	unsigned int count = this->GetPoints();

	if(m_triangles.size() > 0){
		return m_triangles;
	}

	std::vector<Triangle> triangles;

	for(j=0; j<count; j++){
		triangles.push_back(CreateTriangle(j, j+count-1, j+count));
		triangles.push_back(CreateTriangle(j+count, j+1, j));
	}

	/* Horizontal faces */
	PolygonTriangulator *triangulation = nullptr;
	const PolygonTriangulator::Triangles *pg_triangles;
	try{
		std::vector<double> x_coords;
		std::vector<double> y_coords;

		for(unsigned int i=0; i<this->GetPoints(); i++){
			x_coords.insert(x_coords.begin(), this->GetXCoords(i));
			y_coords.insert(y_coords.begin(), this->GetYCoords(i));
		}

		triangulation = new PolygonTriangulator(x_coords, y_coords);
		pg_triangles = triangulation->triangles();
	}catch(int n){
		delete triangulation;

		/* Try reversing the points order */
		std::vector<double> x_coords;
		std::vector<double> y_coords;

		for(unsigned int i=0; i<this->GetPoints(); i++){
			x_coords.insert(x_coords.begin(), this->GetXCoords(this->GetPoints()-1-i));
			y_coords.insert(y_coords.begin(), this->GetYCoords(this->GetPoints()-1-i));
		}

		triangulation = new PolygonTriangulator(x_coords, y_coords);
		pg_triangles = triangulation->triangles();
	}


	for(auto vec = pg_triangles->begin(); vec != pg_triangles->end(); vec++){
		triangles.push_back(CreateTriangle(1*count-vec->at(2), 1*count-vec->at(1), 1*count-vec->at(0)));
		triangles.push_back(CreateTriangle(2*count-vec->at(0), 2*count-vec->at(1), 2*count-vec->at(2)));
	}
	delete triangulation;

	return triangles;
}

}
