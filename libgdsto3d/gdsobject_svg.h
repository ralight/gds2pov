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

#ifndef _GDSOBJECT_SVG_H_
#define _GDSOBJECT_SVG_H_

#include <string>

#include "gdsobject.h"

class GDSObject_svg : public GDSObject
{
private:
	float m_scale;
public:
	GDSObject_svg(std::string name);
	GDSObject_svg(GDSObject *object);
	~GDSObject_svg();

	void SetScale(float scale);

	virtual void OutputToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputPathToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputPolygonToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputTextToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputSRefToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
	void OutputARefToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer);
};

#endif
