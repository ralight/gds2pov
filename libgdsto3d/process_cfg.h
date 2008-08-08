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

#ifndef _PROCESS_CFG_H_
#define _PROCESS_CFG_H_

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
	bool Show;
	float Red;
	float Green;
	float Blue;
	float Filter;
	bool Metal;

	ProcessLayer() : Name(""), Layer(-1), Datatype(-1), 
					Height(0.0), Thickness(0.0), Show(false),
					Red(0.5), Green(0.5), Blue(0.5), Metal(false) {}
};

class GDSProcess
{
private:
	vector<ProcessLayer*> m_layers;
	bool m_valid;		/* Is the process file valid? */
public:
	GDSProcess ();
	~GDSProcess ();

	void Parse(std::string processfile);
	//bool Parse(std::string processfile);

	void AddLayer(ProcessLayer *NewLayer);
	void AddLayer(int Layer, int Datatype);
	ProcessLayer *GetLayer(int Number, int Datatype);
	ProcessLayer *GetLayer();
	ProcessLayer *GetLayer(unsigned int index);
	ProcessLayer *GetLayer(std::string Name);
	int LayerCount();
	bool IsValid();
	float GetHighest();
	float GetLowest();
	bool Save(std::string filename);
};

#endif
