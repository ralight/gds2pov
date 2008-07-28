/*
 * File: process_cfg.h
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

#ifndef _PROCESS_CFG_H
#define _PROCESS_CFG_H

#include <string>
#include <vector>
using namespace std;

class ProcessLayer {
public:
	std::string Name;
	int Layer;
	int Datatype;
	float Height;
	float Thickness;
	int Show;
	float Red;
	float Green;
	float Blue;
	float Filter;
	int Metal;
};

class GDSProcess
{
private:
	vector<class ProcessLayer*> _FirstLayer;
	int _Count;		/* Number of layers found */

	bool _Valid;		/* Is the process file valid? */
public:
	GDSProcess ();
	~GDSProcess ();

	void Parse(std::string processfile);
	//bool Parse(std::string processfile);

	void AddLayer(class ProcessLayer *NewLayer);
	void AddLayer(int Layer, int Datatype);
	class ProcessLayer *GetLayer(int Number, int Datatype);
	class ProcessLayer *GetLayer();
	class ProcessLayer *GetLayer(unsigned int index);
	class ProcessLayer *GetLayer(std::string Name);
	int LayerCount();
	bool IsValid();
	float GetHighest();
	float GetLowest();
	bool Save(std::string filename);
};

#endif // _PROCESS_CFG_H

