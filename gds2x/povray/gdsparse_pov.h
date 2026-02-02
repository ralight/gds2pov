/*
 * File: gdsparse_pov.h
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

#ifndef GDSPARSE_POV_H
#define GDSPARSE_POV_H

#include <string>

#include "gds2x.h"
#include "process_cfg.h"
#include "config_cfg.h"
#include "gdsparse.h"

class GDSParse_pov : public GDS2X::Parse
{
private:
	std::string m_camfile;
	FILE *m_optr;
	GDSConfig *m_config;
public:
	GDSParse_pov(GDS2X::Process *process, GDS2X::option_map_t &options);
	~GDSParse_pov ();

	GDS2X::Object *NewObject(std::string name);
	void OutputHeader();
	void OutputFooter();
};

#endif
