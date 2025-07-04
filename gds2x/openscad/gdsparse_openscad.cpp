/*
 * File: gdsparse_openscad.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the openscad output specific implementation of the GDSParse class.
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


#include <cstdio>

#include "process_cfg.h"
#include "gdsparse.h"
#include "gdsparse_openscad.h"
#include "gdsobject_openscad.h"

extern int verbose_output;

GDSParse_openscad::GDSParse_openscad(GDS2X::Process *process, GDS2X::option_map_t &options) :
		GDS2X::Parse(process, options["generate_process"] == "true")
{
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
	m_outfile = options["outfile"];
	m_format = options["format"];
	m_topcellname = options["topcell"];


	if(options["outfile"] != ""){
		m_optr = fopen(options["outfile"].c_str(), "wt");
		if(!m_optr){
			fprintf(stderr, "Error: Unable to open %s.\n", options["outfile"].c_str());
		}
	}else{
		m_optr = stdout;
	}
}

GDSParse_openscad::~GDSParse_openscad ()
{
	if(m_optr != stdout){
		fclose(m_optr);
	}
}


GDS2X::Object *GDSParse_openscad::NewObject(std::string name)
{
	return new GDSObject_openscad(name, m_optr);
}

void GDSParse_openscad::OutputFooter()
{
	if(m_optr){
		if(m_topcellname.length() > 0){
			fprintf(m_optr, "mod_%s();", m_topcellname.c_str());
		}else{
			if(m_firstobject){
				fprintf(m_optr, "mod_%s();", m_firstobject->GetName().c_str());
			}
		}
	}
}

void GDSParse_openscad::LoadMacroFile(std::string name)
{
	std::unordered_map<std::string, GDS2X::Object *> macros;

	if(name == ""){
		return;
	}

	macros = Parse::LoadMacroFile(name);
	for(auto it=macros.begin(); it!=macros.end(); it++) {
		auto macro = it->second;
		auto object_openscad = new GDSObject_openscad(macro, m_optr);

		m_objects[it->first] = static_cast<GDS2X::Object *>(object_openscad);
	}
}
