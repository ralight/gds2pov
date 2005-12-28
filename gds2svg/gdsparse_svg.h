/*
 * File: gdsparse_svg.h
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

#ifndef __GDSPARSE_SVG_H__
#define __GDSPARSE_SVG_H__

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"

class GDSParse_svg : public GDSParse
{
private:
	float _scale;

public:
	GDSParse_svg (class GDSConfig *config, class GDSProcess *process);
	~GDSParse_svg ();

	//class GDSObject_svg *NewObject(char *Name);
	class GDSObject *NewObject(char *Name);
	void OutputHeader();
	void OutputFooter();
};

#endif // __GDSPARSE_SVG_H__

