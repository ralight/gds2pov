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
	double m_width, m_length;

	bool m_gotboundary;
	bool m_isoutput;

	std::vector<Path*> m_paths;
	std::vector<Text*> m_texts;
	std::vector<Polygon*> m_polygons;

	std::vector<SRefElement*> m_srefs;
	std::vector<ARefElement*> m_arefs;
	Boundary m_boundary;

public:
	Object() { };
	Object(std::string name);
	Object(std::string name, std::vector<Vertex> vertices, std::vector<Triangle> triangles, ProcessLayer *layer);
	virtual ~Object();

	void AddText(double x, double Y, double Z, bool flipped, double mag, int vjust, int hjust, ProcessLayer *layer);
	Text *GetCurrentText();

	void AddPolygon(ProcessLayer *layer);
	Polygon *GetCurrentPolygon();

	void AddSRef(std::string name, double x, double y, bool flipped, double mag);
	void SetSRefRotation(double x, double y, double z);

	void AddARef(std::string name, double x1, double y1, double x2, double y2, double x3, double y3, int columns, int rows, bool flipped, double mag);
	void SetARefRotation(double x, double y, double z);

	void AddPath(int pathtype, double width, double bgnextn, double endextn, ProcessLayer *layer);
	Path *GetCurrentPath();

	std::string GetName();

	virtual void Output() { };

	bool HasASRef();
	SRefElement *GetSRef(unsigned int index);
	ARefElement *GetARef(unsigned int index);
	unsigned int GetSRefCount(void);
	unsigned int GetARefCount(void);

	Boundary *GetBoundary(void);
	double GetWidth();
	double GetLength();

	bool GetIsOutput();

	std::vector<Path*> GetPaths();
	std::vector<Text*> GetTexts();
	std::vector<Polygon*> GetPolygons();

	std::vector<SRefElement*> GetSRefs();
	std::vector<ARefElement*> GetARefs();
};

}
#endif
