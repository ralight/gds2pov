#ifndef __GDSTEXT_H__
#define __GDSTEXT_H__

#include "gdselements.h"
#include "process_cfg.h"

class GDSText
{
private:
	float			_X;
	float			_Y;
	float			_Z;
	float			_Mag;
	int			_VJust;
	int			_HJust;
	struct ProcessLayer	*_Layer;
	char			*_String;
	Transform		_Rotate;
	bool			_Flipped;

public:
	GDSText(float X, float Y, float Z, bool Flipped, float Mag, int VJust, int HJust, struct ProcessLayer *Layer);
	~GDSText();

	void SetString(const char *String);
	void SetRotation(float X, float Y, float Z);

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
