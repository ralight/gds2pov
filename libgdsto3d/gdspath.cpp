#include "gdsobject.h"
#include "gdspath.h"

GDSPath::GDSPath(int Type, float Height, float Thickness, unsigned int Points, float Width, float BgnExtn, float EndExtn, struct ProcessLayer *Layer)
{
	_Type = Type;
	_Coords = new Point[Points];
	_Height = Height;
	_Thickness = Thickness;
	_Points = Points;
	_Width = Width;
	_BgnExtn = BgnExtn;
	_EndExtn = EndExtn;
	_Layer = Layer;
}

GDSPath::~GDSPath()
{
	if(_Coords) delete _Coords;
}

void GDSPath::AddPoint(unsigned int Index, float X, float Y)
{
	if(_Points >= Index){
		_Coords[Index].X = X;
		_Coords[Index].Y = Y;
	}
}


void GDSPath::SetRotation(float X, float Y, float Z)
{
	_Rotate.X = X;
	_Rotate.Y = Y;
	_Rotate.Z = Z;
}

float GDSPath::GetXCoords(unsigned int Index)
{
	return _Coords[Index].X;
}

float GDSPath::GetYCoords(unsigned int Index)
{
	return _Coords[Index].Y;
}

unsigned int GDSPath::GetPoints()
{
	return _Points;
}

float GDSPath::GetHeight()
{
	return _Height;
}

float GDSPath::GetThickness()
{
	return _Thickness;
}

float GDSPath::GetWidth()
{
	return _Width;
}

float GDSPath::GetBgnExtn()
{
	return _BgnExtn;
}

float GDSPath::GetEndExtn()
{
	return _EndExtn;
}

int GDSPath::GetType()
{
	return _Type;
}

struct ProcessLayer *GDSPath::GetLayer()
{
	return _Layer;
}

