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

GDSObject::GDSObject(std::string NewName)
{
	SRefs = NULL;
	ARefs = NULL;

	Name = NewName;

	GotBoundary = false;
	Boundary.XMax = Boundary.YMax = -1000000.0;
	Boundary.XMin = Boundary.YMin =  1000000.0;
	_width = 0.0;
	_height = 0.0;

	IsOutput = false;
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

	if(SRefs){
		delete [] SRefs;
	}

	if(ARefs){
		delete [] ARefs;
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

void GDSObject::AddSRef(std::string Name, float X, float Y, int Flipped, float Mag)
{
	SRefElement *NewSRef = new SRefElement;

	NewSRef->Name = Name;
	NewSRef->X = X;
	NewSRef->Y = Y;
	NewSRef->Rotate.X = 0.0;
	NewSRef->Rotate.Y = 0.0;
	NewSRef->Rotate.Z = 0.0;
	NewSRef->Flipped = Flipped;
	NewSRef->Mag = Mag;
	NewSRef->object = NULL;

	FirstSRef.push_back(NewSRef);
}

void GDSObject::SetSRefRotation(float X, float Y, float Z)
{
	if(!FirstSRef.empty()){
		FirstSRef[FirstSRef.size()-1]->Rotate.X = X;
		FirstSRef[FirstSRef.size()-1]->Rotate.Y = Y;
		FirstSRef[FirstSRef.size()-1]->Rotate.Z = Z;
	}
}

void GDSObject::AddARef(std::string Name, float X1, float Y1, float X2, float Y2, float X3, float Y3, int Columns, int Rows, int Flipped, float Mag)
{
	ARefElement *NewARef = new ARefElement;

	NewARef->Name = Name;
	NewARef->X1 = X1;
	NewARef->Y1 = Y1;
	NewARef->X2 = X2;
	NewARef->Y2 = Y2;
	NewARef->X3 = X3;
	NewARef->Y3 = Y3;
	NewARef->Columns = Columns;
	NewARef->Rows = Rows;
	NewARef->Rotate.X = 0.0;
	NewARef->Rotate.Y = 0.0;
	NewARef->Rotate.Z = 0.0;
	NewARef->Flipped = Flipped;
	NewARef->Mag = Mag;
	NewARef->object = NULL;

	FirstARef.push_back(NewARef);
}

void GDSObject::SetARefRotation(float X, float Y, float Z)
{
	if(!FirstARef.empty()){
		FirstARef[FirstARef.size()-1]->Rotate.X = X;
		FirstARef[FirstARef.size()-1]->Rotate.Y = Y;
		FirstARef[FirstARef.size()-1]->Rotate.Z = Z;
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
				if(polygon->GetXCoords(j) > Boundary.XMax){
					Boundary.XMax = polygon->GetXCoords(j);
				}
				if(polygon->GetXCoords(j) < Boundary.XMin){
					Boundary.XMin = polygon->GetXCoords(j);
				}
				if(polygon->GetYCoords(j) > Boundary.YMax){
					Boundary.YMax = polygon->GetYCoords(j);
				}
				if(polygon->GetYCoords(j) < Boundary.YMin){
					Boundary.YMin = polygon->GetYCoords(j);
				}
			}
		}
	}

	/* FIXME - need to take width into account? */
	if(!PathItems.empty()){
		for(unsigned long i=0; i<PathItems.size(); i++){
			class GDSPath *path = PathItems[i];
			for(unsigned int j=0; j<path->GetPoints(); j++){
				if(path->GetXCoords(j) > Boundary.XMax){
					Boundary.XMax = path->GetXCoords(j);
				}
				if(path->GetXCoords(j) < Boundary.XMin){
					Boundary.XMin = path->GetXCoords(j);
				}
				if(path->GetYCoords(j) > Boundary.YMax){
					Boundary.YMax = path->GetYCoords(j);
				}
				if(path->GetYCoords(j) < Boundary.YMin){
					Boundary.YMin = path->GetYCoords(j);
				}
			}
		}
	}

	for(unsigned int i = 0; i < FirstSRef.size(); i++){
		SRefElement *sref = FirstSRef[i];
		if(Name == sref->Name && sref->object){
			class GDSObject *object = sref->object;
			struct _Boundary *NewBound;
			NewBound = object->GetBoundary();
			if(sref->X + NewBound->XMax > Boundary.XMax){
				Boundary.XMax = sref->X + NewBound->XMax;
			}
			if(sref->X - NewBound->XMin < Boundary.XMin){
				Boundary.XMin = sref->X - NewBound->XMin;
			}
			if(sref->Y + NewBound->YMax > Boundary.YMax){
				Boundary.YMax = sref->Y + NewBound->YMax;
			}
			if(sref->Y - NewBound->YMin < Boundary.YMin){
				Boundary.YMin = sref->Y - NewBound->YMin;
			}
		}
	}

	for(unsigned int i = 0; i < FirstARef.size(); i++){
		ARefElement *aref = FirstARef[i];
		if(Name == aref->Name && aref->object){
			class GDSObject *object = aref->object;
			struct _Boundary *NewBound;
			NewBound = object->GetBoundary();
			if(aref->X2 + NewBound->XMax > Boundary.XMax){
					Boundary.XMax = aref->X2 + NewBound->XMax;
			}
			if(aref->X1 - NewBound->XMin < Boundary.XMin){
				Boundary.XMin = aref->X1 - NewBound->XMin;
			}
			if(aref->Y3 + NewBound->YMax > Boundary.YMax){
				Boundary.YMax = aref->Y3 + NewBound->YMax;
			}
			if(aref->Y1 - NewBound->YMin < Boundary.YMin){
				Boundary.YMin = aref->Y1 - NewBound->YMin;
			}
		}
	}

	if(PathItems.empty() && PolygonItems.empty() && FirstSRef.empty() && FirstARef.empty()){
		Boundary.XMax = Boundary.XMin = Boundary.YMax = Boundary.YMin = 0;
	}

	v_printf(2, "%s\tXMax=%.2f\tXMin=%.2f\tYMax: %.2f\tYMin: %.2f\n", Name.c_str(), Boundary.XMax, Boundary.XMin, Boundary.YMax, Boundary.YMin);
	GotBoundary = true;

	_width = Boundary.XMax - Boundary.XMin;
	_height = Boundary.YMax - Boundary.YMin;

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

int GDSObject::HasASRef()
{
	return (!FirstARef.empty() || !FirstSRef.empty());
}

class GDSObject *GDSObject::GetSRef(unsigned int Index)
{
	if(FirstSRef.size() > 0 && Index < FirstSRef.size()){
		return SRefs[Index];
	}
	return NULL;
}

class GDSObject *GDSObject::GetARef(unsigned int Index)
{
	if(FirstARef.size() > 0 && Index < FirstARef.size()){
		return ARefs[Index];
	}
	return NULL;
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

