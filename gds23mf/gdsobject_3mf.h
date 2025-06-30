/*
 * File: gdsobject_3mf.h
 * Author: Roger Light
 * Project: gdsto3d
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

#ifndef GDSOBJECT_3MF_H
#define GDSOBJECT_3MF_H

#include <string>

#include "gdsobject.h"
#include "gdsparse_3mf.h"
#include <lib3mf_implicit.hpp>

class GDSParse_3mf;

class GDSObject_3mf : public GDS2X::Object
{
private:
	Lib3MF::PWrapper m_wrapper;
	Lib3MF::PModel m_model;
	Lib3MF::PComponentsObject m_root_component;
	Lib3MF::PComponentsObject m_component;
	GDSParse_3mf *m_parse;

	std::vector<sLib3MFPosition> GetPolygonPoints(GDS2X::Polygon *polygon);
	std::vector<sLib3MFTriangle> GetPolygonFaces(GDS2X::Polygon *polygon);
public:
	GDSObject_3mf(std::string name, GDSParse_3mf *parse, Lib3MF::PWrapper wrapper, Lib3MF::PModel model, Lib3MF::PComponentsObject root_component);
	GDSObject_3mf(GDS2X::Object *object, GDSParse_3mf *parse, Lib3MF::PWrapper wrapper, Lib3MF::PModel model, Lib3MF::PComponentsObject root_component);
	~GDSObject_3mf();

	virtual void Output();
	void OutputPaths();
	void OutputPolygons();
	void OutputTexts();
	void OutputSRefs();
	void OutputARefs();

	Lib3MF::PComponentsObject GetComponent();
};

#endif
