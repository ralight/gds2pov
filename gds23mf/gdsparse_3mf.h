/*
 * File: gdsparse_3mf.h
 * Author: Roger Light
 * Project: gds2x
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

#ifndef GDSPARSE_3MF_H
#define GDSPARSE_3MF_H

#include <string>
#include <unordered_map>

#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsobject_3mf.h"
#include <lib3mf_implicit.hpp>

class GDSObject_3mf;

class GDSParse_3mf : public GDS2X::Parse
{
private:
	Lib3MF::PWrapper m_wrapper;
	Lib3MF::PModel m_model;
	Lib3MF::PComponentsObject m_root_component;
	std::unordered_map<std::string, Lib3MF::sTriangleProperties> m_colours;
	void CreateColours(GDS2X::Process *process);

public:
	GDSParse_3mf(GDS2X::Process *process, bool bounding_output, bool generate_process);
	GDSParse_3mf(GDS2X::Parse *parse);
	~GDSParse_3mf ();

	void Output(std::string topcell, std::string outfile, std::string format);
	GDS2X::Object *NewObject(std::string name);
	GDSObject_3mf *GetObjectRef(std::string name);
	void OutputHeader();
	void OutputFooter();
	Lib3MF::sTriangleProperties GetColour(std::string name);
};

#endif
