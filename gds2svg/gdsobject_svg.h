/*
 * File: gdsobject_svg.h
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

#ifndef __GDSOBJECT_SVG_H__
#define __GDSOBJECT_SVG_H__

#include "gdsobject.h"

class GDSObject_svg : public GDSObject
{
private:
	float _scale;
	float _oheight;
public:
	GDSObject_svg(char *Name);
	~GDSObject_svg();

	void SetScale(float scale);

	virtual void OutputToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputPathToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputPolygonToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputTextToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputSRefToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputARefToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);

};

#endif // __GDSOBJECT_SVG_H__

