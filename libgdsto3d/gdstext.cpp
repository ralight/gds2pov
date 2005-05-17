#include "gdsobject.h"
#include "gdstext.h"

GDSText::GDSText(float X, float Y, float Z, bool Flipped, float Mag, int VJust, int HJust, struct ProcessLayer *Layer)
{
	_String = NULL;

	_X = X;
	_Y = Y;
	_Z = Z;
	_Rotate.X = 0.0;
	_Rotate.Y = 0.0;
	_Rotate.Z = 0.0;
	_Flipped = Flipped;
	_Mag = Mag;
	_HJust = HJust;
	_VJust = VJust;
	_Layer = Layer;
}

GDSText::~GDSText()
{
	if(_String) delete [] _String;
}

void GDSText::SetString(const char *String)
{
	if(_String) delete [] _String;

	_String = new char[strlen(String)+1];
	strcpy(_String, String);
}

void GDSText::SetRotation(float X, float Y, float Z)
{
	_Rotate.X = X;
	_Rotate.Y = Y;
	_Rotate.Z = Z;
}

char *GDSText::GetString()
{
	return _String;
}

float GDSText::GetX()
{
	return _X;
}

float GDSText::GetY()
{
	return _Y;
}

float GDSText::GetZ()
{
	return _Z;
}

float GDSText::GetRY()
{
	return _Rotate.Y;
}

float GDSText::GetMag()
{
	return _Mag;
}

int GDSText::GetVJust()
{
	return _VJust;
}

int GDSText::GetHJust()
{
	return _HJust;
}

bool GDSText::GetFlipped()
{
	return _Flipped;
}

struct ProcessLayer *GDSText::GetLayer()
{
	return _Layer;
}

