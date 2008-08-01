/*
 * File: gdstext.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#ifndef _GDSTEXT_H_
#define _GDSTEXT_H_

#include <string>

#include "gdselements.h"
#include "process_cfg.h"

class GDSText
{
private:
	float _X;
	float _Y;
	float _Z;
	bool _Flipped;
	float _Mag;
	int _VJust;
	int _HJust;
	struct ProcessLayer *_Layer;
	std::string _String;
	Transform _Rotate;

public:
	GDSText(float X, float Y, float Z, bool Flipped, float Mag, int VJust, int HJust, struct ProcessLayer *Layer);
	~GDSText();

	void SetString(std::string String);
	void SetRotation(float X, float Y, float Z);

	std::string GetString();
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

#endif
