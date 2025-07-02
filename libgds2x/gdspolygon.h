/*
 * File: gdspolygon.h
 * Author: Roger Light
 * Project: gds2x
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

#ifndef GDSPOLYGON_H
#define GDSPOLYGON_H

#include "process_cfg.h"

namespace GDS2X {

class Polygon
{
private:
	float m_height;
	float m_thickness;
	unsigned int m_points;
	Point *m_coords;
	Transform m_rotate;
	class ProcessLayer	*m_layer;
	std::string m_layer_name;
	std::vector<Vertex> m_vertices;
	std::vector<Triangle> m_triangles;

public:
	Polygon(float height, float thickness, unsigned int points, ProcessLayer *layer);
	Polygon(std::vector<Vertex> vertices, std::vector<Triangle> triangles, ProcessLayer *layer);
	~Polygon();

	void AddPoint(unsigned int index, float x, float y);
	void SetRotation(float x, float y, float z);

	float GetHeight(void);
	float GetThickness(void);
	unsigned int GetPoints(void);
	float GetXCoords(unsigned int index);
	float GetYCoords(unsigned int index);
	float GetAngleCoords(unsigned int index);
	void SetAngleCoords(unsigned int index, float value);
	class ProcessLayer *GetLayer(void);
	std::vector<Vertex> GetVertices();
	std::vector<Triangle> GetTriangles();
};

}
#endif
