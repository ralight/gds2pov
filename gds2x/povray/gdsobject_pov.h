/*
 * File: gdsobject_pov.h
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

#ifndef GDSOBJECT_POV_H
#define GDSOBJECT_POV_H

#include <string>

#include "gdsobject.h"

class GDSObject_pov : public GDS2X::Object
{
private:
	FILE *m_optr;
public:
	GDSObject_pov(std::string name, FILE *optr);
	GDSObject_pov(GDS2X::Object *object, FILE *optr);
	~GDSObject_pov();

	virtual void Output();
	void OutputPaths();
	void OutputPolygons();
	void OutputTexts();
	void OutputSRefs();
	void OutputARefs();
};

#endif
