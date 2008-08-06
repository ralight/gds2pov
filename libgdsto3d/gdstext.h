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

#include "gdsto3d.h"
#include "process_cfg.h"

class GDSText
{
private:
	float m_x;
	float m_y;
	float m_z;
	bool m_flipped;
	float m_mag;
	int m_vjust;
	int m_hjust;
	struct ProcessLayer *m_layer;
	std::string m_string;
	Transform m_rotate;

public:
	GDSText(float x, float y, float z, bool flipped, float mag, int vjust, int hjust, struct ProcessLayer *layer);
	~GDSText();

	void SetString(std::string String);
	void SetRotation(float x, float y, float z);

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
