/*
 * File: gdsparse_openscad.h
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

#ifndef GDSPARSE_OPENSCAD_H
#define GDSPARSE_OPENSCAD_H

#include <string>

#include "gds2x.h"
#include "process_cfg.h"
#include "gdsparse.h"

class GDSParse_openscad : public GDS2X::Parse
{
private:
	FILE *m_optr;

public:
	GDSParse_openscad(GDS2X::Process *process, GDS2X::option_map_t &options);
	~GDSParse_openscad ();

	GDS2X::Object *NewObject(std::string name);
	void OutputFooter();
	void LoadMacroFile(std::string name);
};

#endif
