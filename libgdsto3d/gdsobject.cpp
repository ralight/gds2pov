/*
 * File: gdsobject.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the GDSObject class which corresponds to a GDS SRef.
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

#include <cstring>

#include "gdsobject.h"
#include "gds_globals.h"

GDSObject::GDSObject(std::string name) :
	m_name(name), m_width(0.0), m_height(0.0), m_gotboundary(false), m_isoutput(false)
{
	m_boundary.xmax = m_boundary.ymax = -1000000.0;
	m_boundary.xmin = m_boundary.ymin =  1000000.0;
}

GDSObject::~GDSObject()
{
	while(!m_polygons.empty()){
		delete m_polygons[m_polygons.size()-1];
		m_polygons.pop_back();
	}

	while(!m_paths.empty()){
		delete m_paths[m_paths.size()-1];
		m_paths.pop_back();
	}

	while(!m_texts.empty()){
		delete m_texts[m_texts.size()-1];
		m_texts.pop_back();
	}

	while(!m_srefs.empty()){
		delete m_srefs[m_srefs.size()-1];
		m_srefs.pop_back();
	}

	while(!m_arefs.empty()){
		delete m_arefs[m_arefs.size()-1];
		m_arefs.pop_back();
	}
}

void GDSObject::AddText(float x, float y, float z, bool flipped, float mag, int vjust, int hjust, struct ProcessLayer *layer)
{
	m_texts.push_back(new GDSText(x, y, z, flipped, mag, vjust, hjust, layer));
}

GDSText *GDSObject::GetCurrentText()
{
	if(m_texts.size()){
		return m_texts[m_texts.size()-1];
	}else{
		return NULL;
	}
}

std::string GDSObject::GetName()
{
	return m_name;
}

void GDSObject::AddPolygon(float height, float thickness, int points, struct ProcessLayer *layer)
{
	m_polygons.push_back(new GDSPolygon(height, thickness, points, layer));
}

GDSPolygon *GDSObject::GetCurrentPolygon()
{
	return m_polygons[m_polygons.size()-1];
}

void GDSObject::AddSRef(std::string name, float x, float y, bool flipped, float mag)
{
	ASRefElement *sref = new ASRefElement;

	sref->name = name;
	sref->x1 = x;
	sref->y1 = y;
	sref->flipped = flipped;
	sref->mag = mag;

	m_srefs.push_back(sref);
}

void GDSObject::SetSRefRotation(float x, float y, float z)
{
	if(!m_srefs.empty()){
		m_srefs[m_srefs.size()-1]->rotate.x = x;
		m_srefs[m_srefs.size()-1]->rotate.y = y;
		m_srefs[m_srefs.size()-1]->rotate.z = z;
	}
}

void GDSObject::AddARef(std::string name, float x1, float y1, float x2, float y2, float x3, float y3, int columns, int rows, bool flipped, float mag)
{
	ASRefElement *aref = new ASRefElement;

	aref->name = name;
	aref->x1 = x1;
	aref->y1 = y1;
	aref->x2 = x2;
	aref->y2 = y2;
	aref->x3 = x3;
	aref->y3 = y3;
	aref->columns = columns;
	aref->rows = rows;
	aref->flipped = flipped;
	aref->mag = mag;

	m_arefs.push_back(aref);
}

void GDSObject::SetARefRotation(float x, float y, float z)
{
	if(!m_arefs.empty()){
		m_arefs[m_arefs.size()-1]->rotate.x = x;
		m_arefs[m_arefs.size()-1]->rotate.y = y;
		m_arefs[m_arefs.size()-1]->rotate.z = z;
	}
}

struct _Boundary *GDSObject::GetBoundary(void)
{
	if(m_gotboundary){
		return &m_boundary;
	}

	if(!m_polygons.empty()){
		for(unsigned long i=0; i<m_polygons.size(); i++){
			GDSPolygon *polygon = m_polygons[i];
			for(unsigned int j=0; j<polygon->GetPoints(); j++){
				if(polygon->GetXCoords(j) > m_boundary.xmax){
					m_boundary.xmax = polygon->GetXCoords(j);
				}
				if(polygon->GetXCoords(j) < m_boundary.xmin){
					m_boundary.xmin = polygon->GetXCoords(j);
				}
				if(polygon->GetYCoords(j) > m_boundary.ymax){
					m_boundary.ymax = polygon->GetYCoords(j);
				}
				if(polygon->GetYCoords(j) < m_boundary.ymin){
					m_boundary.ymin = polygon->GetYCoords(j);
				}
			}
		}
	}

	/* FIXME - need to take width into account? */
	if(!m_paths.empty()){
		for(unsigned long i=0; i<m_paths.size(); i++){
			GDSPath *path = m_paths[i];
			for(unsigned int j=0; j<path->GetPoints(); j++){
				if(path->GetXCoords(j) > m_boundary.xmax){
					m_boundary.xmax = path->GetXCoords(j);
				}
				if(path->GetXCoords(j) < m_boundary.xmin){
					m_boundary.xmin = path->GetXCoords(j);
				}
				if(path->GetYCoords(j) > m_boundary.ymax){
					m_boundary.ymax = path->GetYCoords(j);
				}
				if(path->GetYCoords(j) < m_boundary.ymin){
					m_boundary.ymin = path->GetYCoords(j);
				}
			}
		}
	}

	for(unsigned int i = 0; i < m_srefs.size(); i++){
		ASRefElement *sref = m_srefs[i];
		if(m_name == sref->name && sref->object){
			GDSObject *object = sref->object;
			struct _Boundary *NewBound;
			NewBound = object->GetBoundary();
			if(sref->x1 + NewBound->xmax > m_boundary.xmax){
				m_boundary.xmax = sref->x1 + NewBound->xmax;
			}
			if(sref->x1 - NewBound->xmin < m_boundary.xmin){
				m_boundary.xmin = sref->x1 - NewBound->xmin;
			}
			if(sref->y1 + NewBound->ymax > m_boundary.ymax){
				m_boundary.ymax = sref->y1 + NewBound->ymax;
			}
			if(sref->y1 - NewBound->ymin < m_boundary.ymin){
				m_boundary.ymin = sref->y1 - NewBound->ymin;
			}
		}
	}

	for(unsigned int i = 0; i < m_arefs.size(); i++){
		ASRefElement *aref = m_arefs[i];
		if(m_name == aref->name && aref->object){
			GDSObject *object = aref->object;
			struct _Boundary *NewBound;
			NewBound = object->GetBoundary();
			if(aref->x2 + NewBound->xmax > m_boundary.xmax){
				m_boundary.xmax = aref->x2 + NewBound->xmax;
			}
			if(aref->x1 - NewBound->xmin < m_boundary.xmin){
				m_boundary.xmin = aref->x1 - NewBound->xmin;
			}
			if(aref->y3 + NewBound->ymax > m_boundary.ymax){
				m_boundary.ymax = aref->y3 + NewBound->ymax;
			}
			if(aref->y1 - NewBound->ymin < m_boundary.ymin){
				m_boundary.ymin = aref->y1 - NewBound->ymin;
			}
		}
	}

	if(m_paths.empty() && m_polygons.empty() && m_srefs.empty() && m_arefs.empty()){
		m_boundary.xmax = m_boundary.xmin = m_boundary.ymax = m_boundary.ymin = 0;
	}

	v_printf(2, "%s\tXMax=%.2f\tXMin=%.2f\tYMax: %.2f\tYMin: %.2f\n", 
			m_name.c_str(),
			m_boundary.xmax, m_boundary.xmin, m_boundary.ymax, m_boundary.ymin);
	m_gotboundary = true;

	m_width = m_boundary.xmax - m_boundary.xmin;
	m_height = m_boundary.ymax - m_boundary.ymin;

	return &m_boundary;
}

void GDSObject::AddPath(int PathType, float Height, float Thickness, int Points, float Width, float BgnExtn, float EndExtn, struct ProcessLayer *layer)
{
	m_paths.push_back(new GDSPath(PathType, Height, Thickness, Points, Width, BgnExtn, EndExtn, layer));
}

GDSPath *GDSObject::GetCurrentPath()
{
	return m_paths[m_paths.size()-1];
}

bool GDSObject::HasASRef()
{
	return (!m_arefs.empty() || !m_srefs.empty());
}

ASRefElement *GDSObject::GetSRef(unsigned int index)
{
	if(m_srefs.size() > 0 && index < m_srefs.size()){
		return m_srefs[index];
	}
	return NULL;
}

ASRefElement *GDSObject::GetARef(unsigned int index)
{
	if(m_arefs.size() > 0 && index < m_arefs.size()){
		return m_arefs[index];
	}
	return NULL;
}

unsigned int GDSObject::GetSRefCount(void)
{
	return m_srefs.size();
}

unsigned int GDSObject::GetARefCount(void)
{
	return m_arefs.size();
}

bool GDSObject::GetIsOutput()
{
	return m_isoutput;
}

float GDSObject::GetWidth()
{
	return m_width;
}

float GDSObject::GetHeight()
{
	return m_height;
}

