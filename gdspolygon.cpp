#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gds_globals.h"
#include "gdsobject.h"
#include "gdspolygon.h"
//#include "process_cfg.h"

GDSPolygon::GDSPolygon(float newHeight, float newThickness, unsigned int newPoints, struct ProcessLayer *newlayer)
{
	Coords = NULL;
	Coords = new Point[newPoints+1]; //FIXME - debug +1
	Height = newHeight;
	Thickness = newThickness;
	Points = newPoints;
	layer = newlayer;
}

GDSPolygon::~GDSPolygon()
{
	if(Coords) delete Coords;
}

void GDSPolygon::AddPoint(unsigned int Index, float X, float Y)
{
	if(Points >= Index){
		Coords[Index].X = X;
		Coords[Index].Y = Y;
	}
}


void GDSPolygon::SetRotation(float X, float Y, float Z)
{
	Rotate.X = X;
	Rotate.Y = Y;
	Rotate.Z = Z;
}

float GDSPolygon::GetXCoords(unsigned int Index)
{
	return Coords[Index].X;
}

float GDSPolygon::GetYCoords(unsigned int Index)
{
	return Coords[Index].Y;
}

float GDSPolygon::GetAngleCoords(unsigned int Index)
{
	return Coords[Index].Angle;
}

void GDSPolygon::SetAngleCoords(unsigned int Index, float value)
{
	Coords[Index].Angle = value;
}

unsigned int GDSPolygon::GetPoints()
{
	return Points;
}

float GDSPolygon::GetHeight()
{
	return Height;
}

float GDSPolygon::GetThickness()
{
	return Thickness;
}

struct ProcessLayer *GDSPolygon::GetLayer()
{
	return layer;
}



