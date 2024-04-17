/*
 * File: gdsparse_pov.h
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

#ifndef _GDSPARSE_POV_H_
#define _GDSPARSE_POV_H_

#include <string>

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"

class GDSParse_pov : public GDSParse
{
private:
	std::string m_camfile;
public:
	GDSParse_pov (GDSConfig *config, GDSProcess *process, bool bounding_output, std::string camfile, bool generate_process);
	GDSParse_pov(const GDSParse *parse);
	~GDSParse_pov ();

	GDSObject *NewObject(std::string name);
	void OutputHeader();
	void OutputFooter();

	void Decompose(bool value);
};

#endif
