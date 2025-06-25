/*
 * File: gdsobject_pov.h
 * Author: Roger Light
 * Project: gdsto3d
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

#ifndef _GDSOBJECT_POV_H_
#define _GDSOBJECT_POV_H_

#include <string>

#include "gdsobject.h"

class GDSObject_pov : public GDSObject
{
private:
	bool m_decompose;
	FILE *m_optr;

	void DecomposePOVPolygons();
public:
	GDSObject_pov(std::string name, FILE *optr);
	GDSObject_pov(GDSObject *object, FILE *optr);
	~GDSObject_pov();

	void Decompose(bool value);
	virtual void Output(float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputPathToFile(float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputPolygonToFile(float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputTextToFile(float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputSRefToFile(float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputARefToFile(float offx, float offy, long *objectid, class ProcessLayer *firstlayer);

};

#endif
