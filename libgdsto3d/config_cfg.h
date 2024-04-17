/*
 * File: config_cfg.h
 * Author: Roger Light
 * Project gdsto3d
 *
 * Copyright (C) Roger Light.
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

	Position m_camerapos;
	Position m_lookatpos;
	vector<Position *> m_lights;
	Position *m_currentlight;

public:
	GDSConfig(std::string filename="");
	~GDSConfig();

	std::string GetProcessFile();
	float GetAmbient();
	float GetScale();
	bool IsValid();
	std::string GetFont();

	Position *GetLookAtPos();
	Position *GetCameraPos();
	Position *GetLightPos();
	Position *GetLightPos(unsigned int index);
	int GetLightCount();
	void ReadFile(std::string configfile);
};

#endif
