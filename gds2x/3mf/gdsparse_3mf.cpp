/*
 * File: gdsparse_3mf.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the 3MF output specific implementation of the GDSParse class.
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
#include "gdsparse_3mf.h"
#include "gdsobject_3mf.h"

#include <lib3mf_implicit.hpp>

using namespace Lib3MF;

extern int verbose_output;

sLib3MFTriangleProperties fnCreateTriangleColor(PColorGroup colorGroup, Lib3MF_uint32 colorID1, Lib3MF_uint32 colorID2, Lib3MF_uint32 colorID3)
{
	sLib3MFTriangleProperties sTriangleProperty;
	sTriangleProperty.m_ResourceID = colorGroup->GetResourceID();
	sTriangleProperty.m_PropertyIDs[0] = colorID1;
	sTriangleProperty.m_PropertyIDs[1] = colorID2;
	sTriangleProperty.m_PropertyIDs[2] = colorID3;
	return sTriangleProperty;
}


GDSParse_3mf::GDSParse_3mf(GDS2X::Process *process, GDS2X::option_map_t &options) :
		GDS2X::Parse(process, options["generate_process"] == "true")
{
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;

	m_wrapper = CWrapper::loadLibrary();
	m_model = m_wrapper->CreateModel();
	m_root_component = m_model->AddComponentsObject();

	CreateColours(m_process);

	m_outfile = options["outfile"];
	m_format = options["format"];
	m_topcellname = options["topcell"];
}


void GDSParse_3mf::CreateColours(GDS2X::Process *process)
{
	PColorGroup cg = m_model->AddColorGroup();

	for(int i=0; i<process->LayerCount(); i++){
		GDS2X::ProcessLayer *layer = process->GetLayer(i);
		Lib3MF_uint32 col = cg->AddColor(m_wrapper->RGBAToColor(
					layer->Red*255, layer->Green*255, layer->Blue*255, 255));
		sLib3MFTriangleProperties colprop = fnCreateTriangleColor(cg, col, col, col);
		m_colours.insert({layer->Name, colprop});
	}
}

GDSParse_3mf::~GDSParse_3mf ()
{
}


GDS2X::Object *GDSParse_3mf::NewObject(std::string name)
{
	return new GDSObject_3mf(name, this, m_wrapper, m_model, m_root_component);
}


GDSObject_3mf *GDSParse_3mf::GetObjectRef(std::string name)
{
	return static_cast<GDSObject_3mf *>(m_objects[name]);
}


void GDSParse_3mf::Output()
{
	GDS2X::Parse::Output();

	GDSObject_3mf *oref;
	if(m_topcellname.length() > 0){
		oref = GetObjectRef(m_topcellname);
	}else{
		oref = static_cast<GDSObject_3mf *>(m_objects[0]);
	}
	m_root_component->AddComponent(oref->GetComponent().get(), m_wrapper->GetIdentityTransform());

	m_model->AddBuildItem(m_root_component.get(), m_wrapper->GetIdentityTransform());

	PWriter writer3mf = m_model->QueryWriter(m_format);
	writer3mf->WriteToFile(m_outfile);
}

Lib3MF::sTriangleProperties GDSParse_3mf::GetColour(std::string name)
{
	return m_colours[name];
}


void GDSParse_3mf::LoadMacroFile(std::string name)
{
	std::unordered_map<std::string, GDS2X::Object *> macros;

	if(name == ""){
		return;
	}

	macros = Parse::LoadMacroFile(name);
	for(auto it=macros.begin(); it!=macros.end(); it++) {
		auto macro = it->second;
		auto object_3mf = new GDSObject_3mf(macro, this, m_wrapper, m_model, m_root_component);

		m_objects[it->first] = static_cast<GDS2X::Object *>(object_3mf);
	}
}
