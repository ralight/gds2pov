#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gds_globals.h"
#include "gdsobject.h"
#include "gdspath.h"
//#include "process_cfg.h"

GDSPath::GDSPath(int PathType, float newHeight, float newThickness, unsigned int newPoints, float newWidth, float newBgnExtn, float newEndExtn, struct ProcessLayer *newlayer)
{
	Type = PathType;
	Coords = new Point[newPoints];
	Height = newHeight;
	Thickness = newThickness;
	Points = newPoints;
	Width = newWidth;
	BgnExtn = newBgnExtn;
	EndExtn = newEndExtn;
	layer = newlayer;
}

GDSPath::~GDSPath()
{
	if(Coords) delete Coords;
}

void GDSPath::AddPoint(unsigned int Index, float X, float Y)
{
	if(Points >= Index){
		Coords[Index].X = X;
		Coords[Index].Y = Y;
	}
}


void GDSPath::SetRotation(float X, float Y, float Z)
{
	Rotate.X = X;
	Rotate.Y = Y;
	Rotate.Z = Z;
}

float GDSPath::GetXCoords(unsigned int Index)
{
	return Coords[Index].X;
}

float GDSPath::GetYCoords(unsigned int Index)
{
	return Coords[Index].Y;
}

unsigned int GDSPath::GetPoints()
{
	return Points;
}

float GDSPath::GetHeight()
{
	return Height;
}

float GDSPath::GetThickness()
{
	return Thickness;
}

float GDSPath::GetWidth()
{
	return Width;
}

float GDSPath::GetBgnExtn()
{
	return BgnExtn;
}

float GDSPath::GetEndExtn()
{
	return EndExtn;
}

int GDSPath::GetType()
{
	return Type;
}

struct ProcessLayer *GDSPath::GetLayer()
{
	return layer;
}

