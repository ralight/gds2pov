#ifndef __GDSPOLYGON_H__
#define __GDSPOLYGON_H__

#include "process_cfg.h"

class GDSPolygon
{
private:
	float			_Height;
	float			_Thickness;
	unsigned int		_Points;
	Point			*_Coords;
	Transform		_Rotate;
	struct ProcessLayer	*_Layer;

public:
	GDSPolygon(float Height, float Thickness, unsigned int Points, struct ProcessLayer *Layer);
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
