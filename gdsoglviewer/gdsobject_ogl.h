/*
 * File: gdsobject_ogl.h
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

#ifndef __GDSOBJECT_OGL_H__
#define __GDSOBJECT_OGL_H__

#include "gdsobject.h"

class GDSObject_ogl : public GDSObject
{
private:
	void DecomposePolygons(class GDSPolygon *polygon);
public:
	GDSObject_ogl(char *Name);
	~GDSObject_ogl();

	virtual void OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputOGLVertices(float offx, float offy);
	void OutputOGLSRefs(class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputOGLARefs(class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);

};

#endif // __GDSOBJECT_OGL_H__

