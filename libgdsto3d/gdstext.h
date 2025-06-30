/*
 * File: gdstext.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GDSTEXT_H
#define GDSTEXT_H

#include <string>

#include "gdsto3d.h"
#include "process_cfg.h"

namespace GDS2X {

class Text
{
private:
	float m_x;
	float m_y;
	float m_z;
	bool m_flipped;
	float m_mag;
	int m_vjust;
	int m_hjust;
	class ProcessLayer *m_layer;
	std::string m_string;
	Transform m_rotate;

public:
	Text(float x, float y, float z, bool flipped, float mag, int vjust, int hjust, class ProcessLayer *layer);
	~Text();

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

	class ProcessLayer *GetLayer();
};

}
#endif
