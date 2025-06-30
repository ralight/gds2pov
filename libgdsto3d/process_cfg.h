/*
 * File: process_cfg.h
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

#ifndef PROCESS_CFG_H
#define PROCESS_CFG_H

#include <string>
#include <vector>

namespace GDS2X {

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

	void HexColour(char *buf, size_t buflen);
};

class Process
{
private:
	std::vector<ProcessLayer*> m_layers;
	bool m_valid;		/* Is the process file valid? */
public:
	Process ();
	~Process ();

	void Parse(std::string processfile);

	ProcessLayer *AddLayer(ProcessLayer *NewLayer);
	ProcessLayer *AddLayer(int Layer, int Datatype);
	ProcessLayer *GetLayer(int Number, int Datatype);
	ProcessLayer *GetLayer();
	ProcessLayer *GetLayer(unsigned int index);
	ProcessLayer *GetLayer(std::string Name);
	unsigned int LayerCount();
	bool IsValid();
	float GetHighest();
	float GetLowest();
	bool RemoveLayer(unsigned int index);
	bool Save(std::string filename);
	void SwapLayers(unsigned int a, unsigned int b);
};

}
#endif
