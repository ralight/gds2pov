#ifndef __GDSPOLYGON_H__
#define __GDSPOLYGON_H__

#include <stdarg.h>
#include <string>

using namespace std;

#include "process_cfg.h"

class GDSPolygon
{
private:
	float Height;
	float Thickness;
	unsigned int Points;
	Point *Coords;
	Transform Rotate;
	struct ProcessLayer *layer;

public:
	GDSPolygon(float newHeight, float newThickness, unsigned int newPoints, struct ProcessLayer *newlayer);
	~GDSPolygon();

	void AddPoint(unsigned int Index, float X, float Y);
	void SetRotation(float X, float Y, float Z);

	float GetHeight();
	float GetThickness();
	unsigned int GetPoints();
	float GetXCoords(unsigned int Index);
	float GetYCoords(unsigned int Index);
	float GetAngleCoords(unsigned int Index);
	void SetAngleCoords(unsigned int Index, float value);
	struct ProcessLayer *GetLayer();
};

#endif // __GDSPOLYGON_H__
