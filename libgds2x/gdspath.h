/*
 * File: gdspath.h
 * Author: Roger Light
 * Project: gds2x
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

#ifndef GDSPATH_H
#define GDSPATH_H

#include <gds2x.h>
#include "process_cfg.h"

namespace GDS2X {

class Path
{
private:
	int m_type;
	float m_width;
	float m_bgnextn;
	float m_endextn;
	std::vector<Point> m_coords;
	Transform m_rotate;
	ProcessLayer *m_layer;

public:
	Path(int pathtype, float width, float bgnextn, float endextn, ProcessLayer *layer);
	~Path();

	void AddPoint(float x, float y);
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
	ProcessLayer *GetLayer();

	int GetPointCentre(unsigned int idx, float &x, float &y);
	int GetPoint2D(unsigned int idx, float &x, float &y);
	int GetPoint3D(unsigned int idx, float &x, float &y, float &z);
	int GetFace3D(unsigned int idx, int &v1, int &v2, int &v3);
};

}
#endif
