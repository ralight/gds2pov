#ifndef __GDSTEXT_H__
#define __GDSTEXT_H__

#include <stdarg.h>
#include <string>
using namespace std;

#include "gdselements.h"
#include "process_cfg.h"

class GDSText
{
private:
	float X;
	float Y;
	float Z;
	float Mag;
	int VJust;
	int HJust;
	struct ProcessLayer *layer;
	char *String;
	Transform Rotate;
	bool Flipped;

public:
	GDSText(float newX, float newY, float newZ, bool newFlipped, float newMag, int newVJust, int newHJust, struct ProcessLayer *newlayer);
	~GDSText();

	void SetString(const char *newString);
	void SetRotation(float newX, float newY, float newZ);

	char *GetString();
	float GetX();
	float GetY();
	float GetZ();
	float GetRY();
	float GetMag();

	int GetVJust();
	int GetHJust();

	bool GetFlipped();

	struct ProcessLayer *GetLayer();
};

#endif // __GDSTEXT_H__
