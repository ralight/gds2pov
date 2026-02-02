/*
 * File: gdsobject_openscad.h
 * Author: Roger Light
 * Project: gds2x
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

#ifndef GDSOBJECT_OPENSCAD_H
#define GDSOBJECT_OPENSCAD_H

#include <string>

#include "gdsobject.h"

class GDSObject_openscad : public GDS2X::Object
{
private:
	FILE *m_optr;

	void WritePolygonPoints(GDS2X::Polygon *polygon);
	void WritePolygonFaces(GDS2X::Polygon *polygon);
public:
	GDSObject_openscad(std::string name, FILE *optr);
	GDSObject_openscad(GDS2X::Object *object, FILE *optr);
	~GDSObject_openscad();

	virtual void Output();
	void OutputPaths();
	void OutputPolygons();
	void OutputTexts();
	void OutputSRefs();
	void OutputARefs();
};

#endif
