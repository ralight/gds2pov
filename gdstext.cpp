#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdsobject.h"
#include "gdstext.h"
//#include "process_cfg.h"

GDSText::GDSText(float newX, float newY, float newZ, bool newFlipped, float newMag, int newVJust, int newHJust, struct ProcessLayer *newlayer)
{
	String = NULL;

	X = newX;
	Y = newY;
	Z = newZ;
	Rotate.X = 0.0;
	Rotate.Y = 0.0;
	Rotate.Z = 0.0;
	Flipped = newFlipped;
	Mag = newMag;
	HJust = newHJust;
	VJust = newVJust;
	layer = newlayer;
}

GDSText::~GDSText()
{
	if(String) delete String;
}

void GDSText::SetString(const char *newString)
{
	if(String) delete String;

	String = new char[strlen(newString)+1];
	strcpy(String, newString);
}

void GDSText::SetRotation(float newX, float newY, float newZ)
{
	Rotate.X = newX;
	Rotate.Y = newY;
	Rotate.Z = newZ;
}

char *GDSText::GetString()
{
	return String;
}

float GDSText::GetX()
{
	return X;
}

float GDSText::GetY()
{
	return Y;
}

float GDSText::GetZ()
{
	return Z;
}

float GDSText::GetRY()
{
	return Rotate.Y;
}

float GDSText::GetMag()
{
	return Mag;
}

int GDSText::GetVJust()
{
	return VJust;
}

int GDSText::GetHJust()
{
	return HJust;
}

bool GDSText::GetFlipped()
{
	return Flipped;
}

struct ProcessLayer *GDSText::GetLayer()
{
	return layer;
}

