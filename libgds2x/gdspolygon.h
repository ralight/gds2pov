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
	Transform m_rotate;
	ProcessLayer	*m_layer;
	std::string m_layer_name;
	std::vector<Point> m_coords;
	std::vector<Vertex> m_vertices;
	std::vector<Triangle> m_triangles;

public:
	Polygon(ProcessLayer *layer);
	Polygon(std::vector<Vertex> vertices, std::vector<Triangle> triangles, ProcessLayer *layer);
	~Polygon();

	void AddPoint(double x, double y);
	void SetRotation(double x, double y, double z);

	double GetHeight(void);
	double GetThickness(void);
	unsigned int GetPoints(void);
	double GetXCoords(unsigned int index);
	double GetYCoords(unsigned int index);
	double GetAngleCoords(unsigned int index);
	void SetAngleCoords(unsigned int index, double value);
	ProcessLayer *GetLayer(void);
	std::vector<Vertex> GetVertices();
	std::vector<Triangle> GetTriangles();
};

}
#endif
