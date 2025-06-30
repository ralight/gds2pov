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

GDSParse_openscad::GDSParse_openscad(GDS2X::Process *process, FILE *optr,
		bool bounding_output, bool generate_process) :
		GDS2X::Parse(process, generate_process)
{
	m_optr = optr;
	m_bounding_output = bounding_output;
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;
}


GDSParse_openscad::GDSParse_openscad(GDS2X::Parse *parse, FILE *optr)
{
	m_optr = optr;
	m_bounding_output = false; // FIXME
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;

	m_units = parse->GetUnits();
	m_process = parse->GetProcess();

	std::unordered_map<std::string, GDS2X::Object*> objects = parse->GetObjects();
	for(auto it=m_objects.begin(); it!=m_objects.end(); it++) {
		auto object = it->second;
		auto object_openscad = new GDSObject_openscad(object, m_optr);

		m_objects[it->first] = static_cast<GDS2X::Object *>(object_openscad);
	}
}


GDSParse_openscad::~GDSParse_openscad ()
{
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
			if(m_objects[0]){
				fprintf(m_optr, "mod_%s();", m_objects[0]->GetName().c_str());
			}
		}
	}
}

void GDSParse_openscad::OutputHeader()
{
}
