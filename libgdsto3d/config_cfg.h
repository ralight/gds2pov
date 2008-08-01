/*
 * File: config_cfg.h
 * Author: Roger Light
 * Project gdsto3d
 *
 * Copyright (C) Roger Light.
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

#ifndef _CONFIG_CFG_H_
#define _CONFIG_CFG_H_

#include <string>
#include <vector>
using namespace std;

typedef enum {
	ptNone,
	ptCamera,
	ptLookAt,
	ptLight
} PosType;

typedef enum {
	bpCentre,
	bpTopLeft,
	bpTopRight,
	bpBottomLeft,
	bpBottomRight
} BoundaryPos;

class Position {
public: 
	PosType postype;
	BoundaryPos boundarypos;
	float xmod;
	float ymod;
	float zmod;

	Position() : postype(ptLight), boundarypos(bpCentre),
		xmod(1.0), ymod(1.0), zmod(1.0) { };
};

class GDSConfig
{
private:
	std::string m_processfile;
	std::string m_font;
	float m_ambient;
	float m_scale;
	bool m_valid;

	class Position m_camerapos;
	class Position m_lookatpos;
	vector<class Position *> m_lights;
	class Position *m_currentlight;

public:
	GDSConfig(std::string filename="");
	~GDSConfig();

	std::string GetProcessFile();
	float GetAmbient();
	float GetScale();
	bool IsValid();
	std::string GetFont();

	class Position *GetLookAtPos();
	class Position *GetCameraPos();
	class Position *GetLightPos();
	class Position *GetLightPos(int index);
	int GetLightCount();
	void ReadFile(std::string configfile);
};

#endif
