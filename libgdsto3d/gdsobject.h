/*
 * File: gdsobject.h
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

#ifndef _GDSOBJECT_H_
#define _GDSOBJECT_H_

#include <cstdio>
#include <vector>
using namespace std;

#include "gdsto3d.h"
#include "process_cfg.h"
#include "gdspath.h"
#include "gdstext.h"
#include "gdspolygon.h"

class GDSObject
{
protected:
	std::string m_name;
	float m_width, m_height;

	bool m_gotboundary;
	bool m_isoutput;

	vector<GDSPath*> m_paths;
	vector<GDSText*> m_texts;
	vector<GDSPolygon*> m_polygons;

	vector<ASRefElement*> m_srefs;
	vector<ASRefElement*> m_arefs;
	struct _Boundary m_boundary;

public:
	friend class GDSObject_pov;
	friend class GDSObject_svg;

	GDSObject() { };
	GDSObject(std::string name);
	virtual ~GDSObject();

	void AddText(float x, float Y, float Z, bool flipped, float mag, int vjust, int hjust, struct ProcessLayer *layer);
	GDSText *GetCurrentText();

	void AddPolygon(float height, float thickness, int points, struct ProcessLayer *layer);
	GDSPolygon *GetCurrentPolygon();

	void AddSRef(std::string name, float x, float y, bool flipped, float mag);
	void SetSRefRotation(float x, float y, float z);

	void AddARef(std::string name, float x1, float y1, float x2, float y2, float x3, float y3, int columns, int rows, bool flipped, float mag);
	void SetARefRotation(float x, float y, float z);

	void AddPath(int pathtype, float height, float thickness, int points, float width, float bgnextn, float endextn, struct ProcessLayer *layer);
	GDSPath *GetCurrentPath();

	std::string GetName();

	virtual void OutputToFile(FILE *fptr, std::string font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer) { printf("["); };

	bool HasASRef();
	ASRefElement *GetSRef(unsigned int index);
	ASRefElement *GetARef(unsigned int index);
	unsigned int GetSRefCount(void);
	unsigned int GetARefCount(void);

	//struct _Boundary *GetBoundary(struct ObjectList *objectlist);
	struct _Boundary *GetBoundary(void);
	float GetWidth();
	float GetHeight();

	bool GetIsOutput();
};

#endif
