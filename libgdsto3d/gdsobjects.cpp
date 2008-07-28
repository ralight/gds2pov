/*
 * File: gdsobjects.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSObjects class which is a container class for the GDSObject
 * class.
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

#include <cstring>

#include "gdsobjects.h"

GDSObjects::GDSObjects() : Boundary(NULL)
{
}

GDSObjects::~GDSObjects()
{
	while(!FirstObject.empty()){
		delete FirstObject[FirstObject.size()-1];
		FirstObject.pop_back();
	}

	if(Boundary){
		delete Boundary;
	}
}

class GDSObject *GDSObjects::AddObject(std::string Name, class GDSObject *newobject)
{
	FirstObject.push_back(newobject);
	return newobject;
}

class GDSObject *GDSObjects::GetObjectRef(unsigned int Index)
{
	if(!FirstObject.empty() && Index < FirstObject.size()){
		return FirstObject[Index];
	}else{
		return NULL;
	}
}

class GDSObject *GDSObjects::GetObjectRef(std::string Name)
{
	if(!FirstObject.empty() && Name.length() > 0){	
		for(unsigned int i = 0; i < FirstObject.size(); i++){
			if(Name == FirstObject[i]->GetName()){
				return FirstObject[i];
			}
		}
	}
	return NULL;
}


int GDSObjects::GetCount()
{
	return FirstObject.size();
}

struct _Boundary *GDSObjects::GetBoundary()
{
	if(!Boundary){
		Boundary = new struct _Boundary;
	}

	Boundary->XMax = Boundary->YMax = -10000000.0;
	Boundary->XMin = Boundary->YMin =  10000000.0;

	for(unsigned int i = 0; i < FirstObject.size(); i++){
		struct _Boundary *object_bound = FirstObject[i]->GetBoundary();

		if(object_bound->XMax > Boundary->XMax){
			Boundary->XMax = object_bound->XMax;
		}
		if(object_bound->XMin < Boundary->XMin){
			Boundary->XMin = object_bound->XMin;
		}
		if(object_bound->YMax > Boundary->YMax){
			Boundary->YMax = object_bound->YMax;
		}
		if(object_bound->YMin < Boundary->YMin){
			Boundary->YMin = object_bound->YMin;
		}
	}
	return Boundary;
}

