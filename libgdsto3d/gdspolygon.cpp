#include "gdsobject.h"
#include "gdspolygon.h"

GDSPolygon::GDSPolygon(float Height, float Thickness, unsigned int Points, struct ProcessLayer *Layer)
{
	_Coords = NULL;
	_Coords = new Point[Points+1]; //FIXME - debug +1
	_Height = Height;
	_Thickness = Thickness;
	_Points = Points;
	_Layer = Layer;
}

GDSPolygon::~GDSPolygon()
{
	if(_Coords) delete _Coords;
}

void GDSPolygon::AddPoint(unsigned int Index, float X, float Y)
{
	if(_Points >= Index){
		_Coords[Index].X = X;
		_Coords[Index].Y = Y;
	}
}


void GDSPolygon::SetRotation(float X, float Y, float Z)
{
	_Rotate.X = X;
	_Rotate.Y = Y;
	_Rotate.Z = Z;
}

float GDSPolygon::GetXCoords(unsigned int Index)
{
	return _Coords[Index].X;
}

float GDSPolygon::GetYCoords(unsigned int Index)
{
	return _Coords[Index].Y;
}

float GDSPolygon::GetAngleCoords(unsigned int Index)
{
	return _Coords[Index].Angle;
}

void GDSPolygon::SetAngleCoords(unsigned int Index, float Value)
{
	_Coords[Index].Angle = Value;
}

unsigned int GDSPolygon::GetPoints()
{
	return _Points;
}

float GDSPolygon::GetHeight()
{
	return _Height;
}

float GDSPolygon::GetThickness()
{
	return _Thickness;
}

struct ProcessLayer *GDSPolygon::GetLayer()
{
	return _Layer;
}



