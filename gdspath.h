#ifndef __GDSPATH_H__
#define __GDSPATH_H__

#include <stdarg.h>
#include <string>
using namespace std;

#include "process_cfg.h"

class GDSPath
{
private:
	int Type;
	float Height;
	float Thickness;
	unsigned int Points;
	float Width;
	float BgnExtn;
	float EndExtn;
	Point *Coords;
	Transform Rotate;
	struct ProcessLayer *layer;

public:
	GDSPath(int PathType, float Height, float Thickness, unsigned int Points, float Width, float BgnExtn, float EndExtn, struct ProcessLayer *layer);
	~GDSPath();

	void AddPoint(unsigned int Index, float X, float Y);
	void SetRotation(float X, float Y, float Z);

	float GetXCoords(unsigned int Index);
	float GetYCoords(unsigned int Index);
	unsigned int GetPoints();

	float GetHeight();
	float GetThickness();
	float GetWidth();
	float GetBgnExtn();
	float GetEndExtn();

	int GetType();
	struct ProcessLayer *GetLayer();
};

#endif // __GDSPATH_H__

