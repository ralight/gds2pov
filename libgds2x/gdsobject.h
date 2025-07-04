/*
 * File: gdsobject.h
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

#ifndef GDSOBJECT_H
#define GDSOBJECT_H

#include <cstdio>
#include <vector>

#include "gds2x.h"
#include "process_cfg.h"
#include "gdspath.h"
#include "gdstext.h"
#include "gdspolygon.h"

namespace GDS2X {

class Object
{
protected:
	std::string m_name;
	float m_width, m_height;

	bool m_gotboundary;
	bool m_isoutput;

	std::vector<Path*> m_paths;
	std::vector<Text*> m_texts;
	std::vector<Polygon*> m_polygons;

	std::vector<ASRefElement*> m_srefs;
	std::vector<ASRefElement*> m_arefs;
	struct Boundary m_boundary;

public:
	Object() { };
	Object(std::string name);
	Object(std::string name, std::vector<Vertex> vertices, std::vector<Triangle> triangles, class ProcessLayer *layer);
	virtual ~Object();

	void AddText(float x, float Y, float Z, bool flipped, float mag, int vjust, int hjust, class ProcessLayer *layer);
	Text *GetCurrentText();

	void AddPolygon(float height, float thickness, int points, class ProcessLayer *layer);
	Polygon *GetCurrentPolygon();

	void AddSRef(std::string name, float x, float y, bool flipped, float mag);
	void SetSRefRotation(float x, float y, float z);

	void AddARef(std::string name, float x1, float y1, float x2, float y2, float x3, float y3, int columns, int rows, bool flipped, float mag);
	void SetARefRotation(float x, float y, float z);

	void AddPath(int pathtype, float height, float thickness, int points, float width, float bgnextn, float endextn, class ProcessLayer *layer);
	Path *GetCurrentPath();

	std::string GetName();

	virtual void Output() { };

	bool HasASRef();
	ASRefElement *GetSRef(unsigned int index);
	ASRefElement *GetARef(unsigned int index);
	unsigned int GetSRefCount(void);
	unsigned int GetARefCount(void);

	struct Boundary *GetBoundary(void);
	float GetWidth();
	float GetHeight();

	bool GetIsOutput();

	std::vector<Path*> GetPaths();
	std::vector<Text*> GetTexts();
	std::vector<Polygon*> GetPolygons();

	std::vector<ASRefElement*> GetSRefs();
	std::vector<ASRefElement*> GetARefs();
};

}
#endif
