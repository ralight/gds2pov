/*
 * File: gdsobject.h
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

#ifndef _GDSOBJECT_H_
#define _GDSOBJECT_H_

#include <cstdio>
#include <vector>
using namespace std;

#include "process_cfg.h"
#include "gdselements.h"
#include "gdspath.h"
#include "gdstext.h"
#include "gdspolygon.h"

class GDSObject
{
protected:
	std::string Name;
	float _width, _height;

	bool GotBoundary;
	bool IsOutput;

	vector<class GDSPath*> PathItems;
	vector<class GDSText*> TextItems;
	vector<class GDSPolygon*> PolygonItems;

	vector<ASRefElement*> FirstSRef;
	vector<ASRefElement*> FirstARef;
	struct _Boundary Boundary;

public:
	GDSObject(std::string Name);
	virtual ~GDSObject();

	void AddText(float newX, float newY, float newZ, bool newFlipped, float newMag, int newVJust, int newHJust, struct ProcessLayer *newlayer);
	class GDSText *GetCurrentText();

	void AddPolygon(float Height, float Thickness, int Points, struct ProcessLayer *layer);
	class GDSPolygon *GetCurrentPolygon();

	void AddSRef(std::string Name, float X, float Y, bool Flipped, float Mag);
	void SetSRefRotation(float X, float Y, float Z);

	void AddARef(std::string Name, float X1, float Y1, float X2, float Y2, float X3, float Y3, int Columns, int Rows, bool Flipped, float Mag);
	void SetARefRotation(float X, float Y, float Z);

	void AddPath(int PathType, float Height, float Thickness, int Points, float Width, float BgnExtn, float EndExtn, struct ProcessLayer *layer);
	class GDSPath *GetCurrentPath();

	std::string GetName();

	virtual void OutputToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)=0;

	bool HasASRef();
	ASRefElement *GetSRef(unsigned int Index);
	ASRefElement *GetARef(unsigned int Index);
	unsigned int GetSRefCount(void);
	unsigned int GetARefCount(void);

	//struct _Boundary *GetBoundary(struct ObjectList *objectlist);
	struct _Boundary *GetBoundary(void);
	float GetWidth();
	float GetHeight();

	bool GetIsOutput();
};

#endif
