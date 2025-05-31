/*
 * File: gdsparse_3mf.cpp
 * Author: Roger Light
 * Project: gdsto3d
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


GDSParse_3mf::GDSParse_3mf(GDSProcess *process,
		bool bounding_output, bool generate_process) :
		GDSParse(process, generate_process)
{
	m_bounding_output = bounding_output;
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;

	m_wrapper = CWrapper::loadLibrary();
	m_model = m_wrapper->CreateModel();
	m_root_component = m_model->AddComponentsObject();

	CreateColours(m_process);
}


GDSParse_3mf::GDSParse_3mf(GDSParse *parse)
{
	m_bounding_output = false; // FIXME
	m_use_outfile = true;
	m_allow_multiple_output = false;
	m_output_children_first = true;

	m_units = parse->GetUnits();
	m_process = parse->GetProcess();

	m_wrapper = CWrapper::loadLibrary();
	m_model = m_wrapper->CreateModel();
	m_root_component = m_model->AddComponentsObject();

	CreateColours(m_process);

	vector<GDSObject*> objects = parse->GetObjects();
	for(unsigned int i = 0; i < objects.size(); i++){
		m_objects.push_back(new GDSObject_3mf(objects[i], this, m_wrapper, m_model, m_root_component));
	}
}


void GDSParse_3mf::CreateColours(GDSProcess *process)
{
	PColorGroup cg = m_model->AddColorGroup();

	for(int i=0; i<process->LayerCount(); i++){
		ProcessLayer *layer = process->GetLayer(i);
		Lib3MF_uint32 col = cg->AddColor(m_wrapper->RGBAToColor(
					layer->Red*255, layer->Green*255, layer->Blue*255, 255));
		sLib3MFTriangleProperties colprop = fnCreateTriangleColor(cg, col, col, col);
		m_colours.insert({layer->Name, colprop});
	}
}

GDSParse_3mf::~GDSParse_3mf ()
{
}


GDSObject *GDSParse_3mf::NewObject(std::string name)
{
	return new GDSObject_3mf(name, this, m_wrapper, m_model, m_root_component);
}

void GDSParse_3mf::OutputFooter()
{
}

void GDSParse_3mf::OutputHeader()
{
}


GDSObject_3mf *GDSParse_3mf::GetObjectRef(std::string name)
{
	if(!m_objects.empty() && name.length() > 0){
		for(unsigned int i = 0; i < m_objects.size(); i++){
			if(name == m_objects[i]->GetName()){
				return static_cast<GDSObject_3mf *>(m_objects[i]);
			}
		}
	}
	return nullptr;
}

void GDSParse_3mf::Output(std::string topcell, std::string outfile, std::string format)
{
	GDSParse::Output(topcell);

	GDSObject_3mf *oref;
	if(topcell.length() > 0){
		oref = GetObjectRef(topcell);
	}else{
		oref = static_cast<GDSObject_3mf *>(m_objects[0]);
	}
	m_root_component->AddComponent(oref->GetComponent().get(), m_wrapper->GetIdentityTransform());

	m_model->AddBuildItem(m_root_component.get(), m_wrapper->GetIdentityTransform());

	PWriter writer3mf = m_model->QueryWriter(format);
	writer3mf->WriteToFile(outfile);
}

Lib3MF::sTriangleProperties GDSParse_3mf::GetColour(std::string name)
{
	return m_colours[name];
}
