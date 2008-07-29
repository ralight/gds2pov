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

#ifndef _CONFIG_CFG_H
#define _CONFIG_CFG_H

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
	float XMod;
	float YMod;
	float ZMod;

	Position() : postype(ptLight), boundarypos(bpCentre),
		XMod(1.0), YMod(1.0), ZMod(1.0) { };
};

class GDSConfig
{
private:
	std::string _ProcessFile;
	std::string _Font;
	float _Ambient;
	float _Scale;
	bool _Valid;

	class Position _CameraPos;
	class Position _LookAtPos;
	vector<class Position *> _Lights;
	class Position *_currentlight;

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

#endif // _PROCESS_CFG_H

