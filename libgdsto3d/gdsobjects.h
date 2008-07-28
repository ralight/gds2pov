/*
 * File: gdsobjects.h
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

#ifndef __GDSOBJECTS_H__
#define __GDSOBJECTS_H__

#include <vector>
#include <string>

#include "gdsobject.h"

class GDSObjects
{
private:
	vector<class GDSObject *>FirstObject;
	struct _Boundary *Boundary;

public:
	GDSObjects();
	~GDSObjects();

	class GDSObject *AddObject(std::string Name, class GDSObject *newobject);
	class GDSObject *GetObjectRef(unsigned int Index);
	class GDSObject *GetObjectRef(std::string Name);
	struct _Boundary *GetBoundary();
	int GetCount();
};

#endif // __GDSOBJECTS_H__
