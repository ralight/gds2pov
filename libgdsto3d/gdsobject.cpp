/*
 * File: gdsobject.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSObject class which corresponds to a GDS SRef.
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

#include "gdsobject.h"
#include "gds_globals.h"

GDSObject::GDSObject(std::string NewName) :
	Name(NewName), _width(0.0), _height(0.0), GotBoundary(false), IsOutput(false)
{
	Boundary.xmax = Boundary.ymax = -1000000.0;
	Boundary.xmin = Boundary.ymin =  1000000.0;
}

GDSObject::~GDSObject()
{
	while(!PolygonItems.empty()){
		delete PolygonItems[PolygonItems.size()-1];
		PolygonItems.pop_back();
	}

	while(!PathItems.empty()){
		delete PathItems[PathItems.size()-1];
		PathItems.pop_back();
	}

	while(!TextItems.empty()){
		delete TextItems[TextItems.size()-1];
		TextItems.pop_back();
	}

	while(!FirstSRef.empty()){
		delete FirstSRef[FirstSRef.size()-1];
		FirstSRef.pop_back();
	}

	while(!FirstARef.empty()){
		delete FirstARef[FirstARef.size()-1];
		FirstARef.pop_back();
	}
}

void GDSObject::AddText(float newX, float newY, float newZ, bool newFlipped, float newMag, int newVJust, int newHJust, struct ProcessLayer *newlayer)
{
	TextItems.push_back(new class GDSText(newX, newY, newZ, newFlipped, newMag, newVJust, newHJust, newlayer));
}

class GDSText *GDSObject::GetCurrentText()
{
	if(TextItems.size()){
		return TextItems[TextItems.size()-1];
	}else{
		return NULL;
	}
}

std::string GDSObject::GetName()
{
	return Name;
}

void GDSObject::AddPolygon(float Height, float Thickness, int Points, struct ProcessLayer *layer)
{
	PolygonItems.push_back(new class GDSPolygon(Height, Thickness, Points, layer));
}

class GDSPolygon *GDSObject::GetCurrentPolygon()
{
	return PolygonItems[PolygonItems.size()-1];
}

void GDSObject::AddSRef(std::string Name, float X, float Y, bool Flipped, float Mag)
{
	ASRefElement *NewSRef = new ASRefElement;

	NewSRef->name = Name;
	NewSRef->x1 = X;
	NewSRef->y1 = Y;
	NewSRef->flipped = Flipped;
	NewSRef->mag = Mag;

	FirstSRef.push_back(NewSRef);
}

void GDSObject::SetSRefRotation(float X, float Y, float Z)
{
	if(!FirstSRef.empty()){
		FirstSRef[FirstSRef.size()-1]->rotate.x = X;
		FirstSRef[FirstSRef.size()-1]->rotate.y = Y;
		FirstSRef[FirstSRef.size()-1]->rotate.z = Z;
	}
}

void GDSObject::AddARef(std::string Name, float x1, float y1, float x2, float y2, float x3, float y3, int Columns, int Rows, bool Flipped, float Mag)
{
	ASRefElement *NewARef = new ASRefElement;

	NewARef->name = Name;
	NewARef->x1 = x1;
	NewARef->y1 = y1;
	NewARef->x2 = x2;
	NewARef->y2 = y2;
	NewARef->x3 = x3;
	NewARef->y3 = y3;
	NewARef->columns = Columns;
	NewARef->rows = Rows;
	NewARef->flipped = Flipped;
	NewARef->mag = Mag;

	FirstARef.push_back(NewARef);
}

void GDSObject::SetARefRotation(float X, float Y, float Z)
{
	if(!FirstARef.empty()){
		FirstARef[FirstARef.size()-1]->rotate.x = X;
		FirstARef[FirstARef.size()-1]->rotate.y = Y;
		FirstARef[FirstARef.size()-1]->rotate.z = Z;
	}
}

struct _Boundary *GDSObject::GetBoundary(void)
{
	if(GotBoundary){
		return &Boundary;
	}

	if(!PolygonItems.empty()){
		for(unsigned long i=0; i<PolygonItems.size(); i++){
			class GDSPolygon *polygon = PolygonItems[i];
			for(unsigned int j=0; j<polygon->GetPoints(); j++){
				if(polygon->GetXCoords(j) > Boundary.xmax){
					Boundary.xmax = polygon->GetXCoords(j);
				}
				if(polygon->GetXCoords(j) < Boundary.xmin){
					Boundary.xmin = polygon->GetXCoords(j);
				}
				if(polygon->GetYCoords(j) > Boundary.ymax){
					Boundary.ymax = polygon->GetYCoords(j);
				}
				if(polygon->GetYCoords(j) < Boundary.ymin){
					Boundary.ymin = polygon->GetYCoords(j);
				}
			}
		}
	}

	/* FIXME - need to take width into account? */
	if(!PathItems.empty()){
		for(unsigned long i=0; i<PathItems.size(); i++){
			class GDSPath *path = PathItems[i];
			for(unsigned int j=0; j<path->GetPoints(); j++){
				if(path->GetXCoords(j) > Boundary.xmax){
					Boundary.xmax = path->GetXCoords(j);
				}
				if(path->GetXCoords(j) < Boundary.xmin){
					Boundary.xmin = path->GetXCoords(j);
				}
				if(path->GetYCoords(j) > Boundary.ymax){
					Boundary.ymax = path->GetYCoords(j);
				}
				if(path->GetYCoords(j) < Boundary.ymin){
					Boundary.ymin = path->GetYCoords(j);
				}
			}
		}
	}

	for(unsigned int i = 0; i < FirstSRef.size(); i++){
		ASRefElement *sref = FirstSRef[i];
		if(Name == sref->name && sref->object){
			class GDSObject *object = sref->object;
			struct _Boundary *NewBound;
			NewBound = object->GetBoundary();
			if(sref->x1 + NewBound->xmax > Boundary.xmax){
				Boundary.xmax = sref->x1 + NewBound->xmax;
			}
			if(sref->x1 - NewBound->xmin < Boundary.xmin){
				Boundary.xmin = sref->x1 - NewBound->xmin;
			}
			if(sref->y1 + NewBound->ymax > Boundary.ymax){
				Boundary.ymax = sref->y1 + NewBound->ymax;
			}
			if(sref->y1 - NewBound->ymin < Boundary.ymin){
				Boundary.ymin = sref->y1 - NewBound->ymin;
			}
		}
	}

	for(unsigned int i = 0; i < FirstARef.size(); i++){
		ASRefElement *aref = FirstARef[i];
		if(Name == aref->name && aref->object){
			class GDSObject *object = aref->object;
			struct _Boundary *NewBound;
			NewBound = object->GetBoundary();
			if(aref->x2 + NewBound->xmax > Boundary.xmax){
					Boundary.xmax = aref->x2 + NewBound->xmax;
			}
			if(aref->x1 - NewBound->xmin < Boundary.xmin){
				Boundary.xmin = aref->x1 - NewBound->xmin;
			}
			if(aref->y3 + NewBound->ymax > Boundary.ymax){
				Boundary.ymax = aref->y3 + NewBound->ymax;
			}
			if(aref->y1 - NewBound->ymin < Boundary.ymin){
				Boundary.ymin = aref->y1 - NewBound->ymin;
			}
		}
	}

	if(PathItems.empty() && PolygonItems.empty() && FirstSRef.empty() && FirstARef.empty()){
		Boundary.xmax = Boundary.xmin = Boundary.ymax = Boundary.ymin = 0;
	}

	v_printf(2, "%s\tXMax=%.2f\tXMin=%.2f\tYMax: %.2f\tYMin: %.2f\n", Name.c_str(), Boundary.xmax, Boundary.xmin, Boundary.ymax, Boundary.ymin);
	GotBoundary = true;

	_width = Boundary.xmax - Boundary.xmin;
	_height = Boundary.ymax - Boundary.ymin;

	return &Boundary;
}

void GDSObject::AddPath(int PathType, float Height, float Thickness, int Points, float Width, float BgnExtn, float EndExtn, struct ProcessLayer *layer)
{
	PathItems.push_back(new class GDSPath(PathType, Height, Thickness, Points, Width, BgnExtn, EndExtn, layer));
}

class GDSPath *GDSObject::GetCurrentPath()
{
	return PathItems[PathItems.size()-1];
}

bool GDSObject::HasASRef()
{
	return (!FirstARef.empty() || !FirstSRef.empty());
}

ASRefElement *GDSObject::GetSRef(unsigned int Index)
{
	if(FirstSRef.size() > 0 && Index < FirstSRef.size()){
		return FirstSRef[Index];
	}
	return NULL;
}

ASRefElement *GDSObject::GetARef(unsigned int Index)
{
	if(FirstARef.size() > 0 && Index < FirstARef.size()){
		return FirstARef[Index];
	}
	return NULL;
}

unsigned int GDSObject::GetSRefCount(void)
{
	return FirstSRef.size();
}

unsigned int GDSObject::GetARefCount(void)
{
	return FirstARef.size();
}

bool GDSObject::GetIsOutput()
{
	return IsOutput;
}

float GDSObject::GetWidth()
{
	return _width;
}

float GDSObject::GetHeight()
{
	return _height;
}

