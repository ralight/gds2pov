/*
 * File: gdsobject.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the GDSObject class which corresponds to a GDS SRef.
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

#include <cstring>
#include <cmath>

#include "gdsobject.h"
#include "gds_globals.h"

using namespace GDS2X;

Object::Object(std::string name) :
	m_name(name), m_width(0.0), m_length(0.0), m_gotboundary(false), m_isoutput(false)
{
	m_boundary.xmax = m_boundary.ymax = -1000000.0;
	m_boundary.xmin = m_boundary.ymin =  1000000.0;
}


Object::Object(std::string name, std::vector<Vertex> vertices, std::vector<Triangle> triangles, class ProcessLayer *layer) :
	m_name(name), m_gotboundary(false), m_isoutput(false)
{
	m_polygons.push_back(new Polygon(vertices, triangles, layer));
}


Object::~Object()
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

void Object::AddText(float x, float y, float z, bool flipped, float mag, int vjust, int hjust, class ProcessLayer *layer)
{
	m_texts.push_back(new Text(x, y, z, flipped, mag, vjust, hjust, layer));
}

Text *Object::GetCurrentText()
{
	if(m_texts.size()){
		return m_texts[m_texts.size()-1];
	}else{
		return NULL;
	}
}

std::string Object::GetName()
{
	return m_name;
}

void Object::AddPolygon(ProcessLayer *layer)
{
	m_polygons.push_back(new Polygon(layer));
}

Polygon *Object::GetCurrentPolygon()
{
	return m_polygons[m_polygons.size()-1];
}

void Object::AddSRef(std::string name, float x, float y, bool flipped, float mag)
{
	SRefElement *sref = new SRefElement;

	sref->name = name;
	sref->x = x;
	sref->y = y;
	sref->flipped = flipped;
	sref->mag = mag;

	m_srefs.push_back(sref);
}

void Object::SetSRefRotation(float x, float y, float z)
{
	if(!m_srefs.empty()){
		m_srefs[m_srefs.size()-1]->rotate.x = x;
		m_srefs[m_srefs.size()-1]->rotate.y = y;
		m_srefs[m_srefs.size()-1]->rotate.z = z;
	}
}

void Object::AddARef(std::string name, float x1, float y1, float x2, float y2, float x3, float y3, int columns, int rows, bool flipped, float mag)
{
	ARefElement *aref = new ARefElement;

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

void Object::SetARefRotation(float x, float y, float z)
{
	if(!m_arefs.empty()){
		m_arefs[m_arefs.size()-1]->rotate.x = x;
		m_arefs[m_arefs.size()-1]->rotate.y = y;
		m_arefs[m_arefs.size()-1]->rotate.z = z;
	}
}

Boundary *Object::GetBoundary(void)
{
	if(m_gotboundary){
		return &m_boundary;
	}

	if(!m_polygons.empty()){
		for(unsigned long i=0; i<m_polygons.size(); i++){
			Polygon *polygon = m_polygons[i];
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
			Path *path = m_paths[i];
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
		SRefElement *sref = m_srefs[i];
		if(m_name == sref->name && sref->object){
			Object *object = sref->object;
			Boundary *NewBound;
			NewBound = object->GetBoundary();

			float xmax = 0.0, xmin = 0.0;
			float ymax = 0.0, ymin = 0.0;

			switch((int)round(sref->rotate.y)){
				case 0:
				case 360:
				case 180:
				case -180:
					xmax = NewBound->xmax;
					xmin = NewBound->xmin;
					ymax = NewBound->ymax;
					ymin = NewBound->ymin;
					break;

				case 90:
				case -270:
				case 270:
				case -90:
					xmax = NewBound->ymax;
					xmin = NewBound->ymin;
					ymax = NewBound->xmax;
					ymin = NewBound->xmin;
					break;
			}

			if(sref->x + xmax > m_boundary.xmax){
				m_boundary.xmax = sref->x + xmax;
			}
			if(sref->x - xmin < m_boundary.xmin){
				m_boundary.xmin = sref->x - xmin;
			}
			if(sref->y + ymax > m_boundary.ymax){
				m_boundary.ymax = sref->y + ymax;
			}
			if(sref->y - ymin < m_boundary.ymin){
				m_boundary.ymin = sref->y - ymin;
			}
		}
	}

	for(unsigned int i = 0; i < m_arefs.size(); i++){
		ARefElement *aref = m_arefs[i];
		if(m_name != aref->name && aref->object){
			Object *object = aref->object;
			Boundary *NewBound;
			NewBound = object->GetBoundary();

			float xmax = 0.0, xmin = 0.0;
			float ymax = 0.0, ymin = 0.0;

			switch((int)round(aref->rotate.y)){
				case 0:
				case 360:
				case 180:
				case -180:
					xmax = NewBound->xmax;
					xmin = NewBound->xmin;
					ymax = NewBound->ymax;
					ymin = NewBound->ymin;
					break;

				case 90:
				case -270:
				case 270:
				case -90:
					xmax = NewBound->ymax;
					xmin = NewBound->ymin;
					ymax = NewBound->xmax;
					ymin = NewBound->xmin;
					break;
			}

			if(aref->x2 + xmax > m_boundary.xmax){
				m_boundary.xmax = aref->x2 + xmax;
			}
			if(aref->x1 - xmin < m_boundary.xmin){
				m_boundary.xmin = aref->x1 - xmin;
			}
			if(aref->y3 + ymax > m_boundary.ymax){
				m_boundary.ymax = aref->y3 + ymax;
			}
			if(aref->y1 - ymin < m_boundary.ymin){
				m_boundary.ymin = aref->y1 - ymin;
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
	m_length = m_boundary.ymax - m_boundary.ymin;

	return &m_boundary;
}

void Object::AddPath(int PathType, float Width, float BgnExtn, float EndExtn, ProcessLayer *layer)
{
	m_paths.push_back(new Path(PathType, Width, BgnExtn, EndExtn, layer));
}

Path *Object::GetCurrentPath()
{
	return m_paths[m_paths.size()-1];
}

bool Object::HasASRef()
{
	return (!m_arefs.empty() || !m_srefs.empty());
}

SRefElement *Object::GetSRef(unsigned int index)
{
	if(m_srefs.size() > 0 && index < m_srefs.size()){
		return m_srefs[index];
	}
	return NULL;
}

ARefElement *Object::GetARef(unsigned int index)
{
	if(m_arefs.size() > 0 && index < m_arefs.size()){
		return m_arefs[index];
	}
	return NULL;
}

unsigned int Object::GetSRefCount(void)
{
	return m_srefs.size();
}

unsigned int Object::GetARefCount(void)
{
	return m_arefs.size();
}

bool Object::GetIsOutput()
{
	return m_isoutput;
}

float Object::GetWidth()
{
	return m_width;
}

float Object::GetLength()
{
	return m_length;
}


std::vector<Path*> Object::GetPaths()
{
	return m_paths;
}

std::vector<Text*> Object::GetTexts()
{
	return m_texts;
}

std::vector<Polygon*> Object::GetPolygons()
{
	return m_polygons;
}


std::vector<SRefElement*> Object::GetSRefs()
{
	return m_srefs;
}

std::vector<ARefElement*> Object::GetARefs()
{
	return m_arefs;
}
