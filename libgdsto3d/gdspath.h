/*
 * File: gdspath.h
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

#ifndef _GDSPATH_H_
#define _GDSPATH_H_

#include "process_cfg.h"

class GDSPath
{
private:
	int m_type;
	float m_height;
	float m_thickness;
	unsigned int m_points;
	float m_width;
	float m_bgnextn;
	float m_endextn;
	Point *m_coords;
	Transform m_rotate;
	class ProcessLayer	*m_layer;

public:
	GDSPath(int pathtype, float height, float thickness, unsigned int points, float width, float bgnextn, float endextn, class ProcessLayer *layer);
	~GDSPath();

	void AddPoint(unsigned int index, float x, float y);
	void SetRotation(float x, float y, float z);

	float GetXCoords(unsigned int index);
	float GetYCoords(unsigned int index);
	unsigned int GetPoints();

	float GetHeight();
	float GetThickness();
	float GetWidth();
	float GetBgnExtn();
	float GetEndExtn();

	int GetType();
	class ProcessLayer *GetLayer();
};

#endif
