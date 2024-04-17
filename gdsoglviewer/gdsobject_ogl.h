/*
 * File: gdsobject_ogl.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GDSOBJECT_OGL_H
#define GDSOBJECT_OGL_H

#include "gdsobject.h"

class GDSObject_ogl : public GDSObject
{
private:
	void DecomposePolygons(class GDSPolygon *polygon);
public:
	GDSObject_ogl(std::string Name);
	~GDSObject_ogl();

	virtual void OutputToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputOGLVertices(float offx, float offy);
	void OutputOGLSRefs(std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputOGLARefs(std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);

};

#endif
