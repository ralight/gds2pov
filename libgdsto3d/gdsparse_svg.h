/*
 * File: gdsparse_svg.h
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

#ifndef _GDSPARSE_SVG_H_
#define _GDSPARSE_SVG_H_

#include <string>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"

class GDSParse_svg : public GDSParse
{
private:
	float m_scale;

public:
	GDSParse_svg (GDSConfig *config, GDSProcess *process, bool generate_process);
	GDSParse_svg(GDSParse *parse);
	~GDSParse_svg ();

	GDSObject *NewObject(std::string name);
	void OutputHeader();
	void OutputFooter();
};

#endif
