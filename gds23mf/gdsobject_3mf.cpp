/*
 * File: gdsobject_3mf.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the 3mf output specific implementation of the GDSObject class.
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
#include <cstring>
#include <cmath>
#include <sys/random.h>

#include "gdsobject_3mf.h"
#include "PolygonTriangulator.h"

using namespace Lib3MF;

sLib3MFPosition CreateVertex3(float x, float y, float z)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = x;
	result.m_Coordinates[1] = y;
	result.m_Coordinates[2] = z;
	return result;
}

sLib3MFTriangle CreateTriangle3(int v1, int v2, int v3)
{
	sLib3MFTriangle result;
	result.m_Indices[0] = v1;
	result.m_Indices[1] = v2;
	result.m_Indices[2] = v3;
	return result;
}


sLib3MFPosition CreateVertex(GDSVertex &v)
{
	sLib3MFPosition result;
	result.m_Coordinates[0] = v.x;
	result.m_Coordinates[1] = v.y;
	result.m_Coordinates[2] = v.z;
	return result;
}

sLib3MFTriangle CreateTriangle(GDSTriangle &t)
{
	sLib3MFTriangle result;
	result.m_Indices[0] = t.v[0];
	result.m_Indices[1] = t.v[1];
	result.m_Indices[2] = t.v[2];
	return result;
}


sLib3MFTransform CreateTransformMatrix(float x, float y, float rotate, bool mirror, float scale)
{
	sLib3MFTransform mMatrix;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			mMatrix.m_Fields[i][j] = (i == j) ? 1.0f : 0.0f;
		}
	}

	mMatrix.m_Fields[3][0] = x;
	mMatrix.m_Fields[3][1] = y;

	mMatrix.m_Fields[0][0] *= cos(3.141592653*rotate/180.0);
	mMatrix.m_Fields[0][1] = -sin(3.141592653*rotate/180.0);
	mMatrix.m_Fields[1][0] = sin(3.141592653*rotate/180.0);
	mMatrix.m_Fields[1][1] *= cos(3.141592653*rotate/180.0);

	mMatrix.m_Fields[1][0] *= mirror?-1.0:1.0;
	mMatrix.m_Fields[1][1] *= mirror?-1.0:1.0;

	mMatrix.m_Fields[0][0] *= scale;
	mMatrix.m_Fields[0][1] *= scale;
	mMatrix.m_Fields[1][0] *= scale;
	mMatrix.m_Fields[1][1] *= scale;

	return mMatrix;
}

GDSObject_3mf::GDSObject_3mf(std::string name, GDSParse_3mf *parse, Lib3MF::PWrapper wrapper, Lib3MF::PModel model, Lib3MF::PComponentsObject root_component) : GDSObject(name)
{
	m_parse = parse;
	m_wrapper = wrapper;
	m_model = model;
	m_root_component = root_component;
	m_component = model->AddComponentsObject();
}


GDSObject_3mf::GDSObject_3mf(GDSObject *object, GDSParse_3mf *parse, Lib3MF::PWrapper wrapper, Lib3MF::PModel model, Lib3MF::PComponentsObject root_component)
{
	m_parse = parse;
	m_wrapper = wrapper;
	m_model = model;
	m_root_component = root_component;
	m_component = model->AddComponentsObject();

	m_name = object->GetName();
	m_width = object->GetWidth();
	m_height = object->GetHeight();

	m_gotboundary = false;
	m_isoutput = false;

	m_paths = object->GetPaths();
	m_texts = object->GetTexts();
	m_polygons = object->GetPolygons();

	m_srefs = object->GetSRefs();
	m_arefs = object->GetARefs();
}


GDSObject_3mf::~GDSObject_3mf()
{
}

void GDSObject_3mf::OutputPaths()
{
	if(!m_paths.empty()){
		GDSPath *path;

		for(unsigned long i=0; i<m_paths.size(); i++){
			path = m_paths[i];

			PMeshObject mesh = m_model->AddMeshObject();
			mesh->SetName(m_name);

			std::vector<sLib3MFPosition> vertices(4+4*path->GetPoints());
			std::vector<sLib3MFTriangle> faces(4+8*(path->GetPoints()-1));

			int idx = 0;
			float x, y, z;
			while(path->GetPoint3D(idx, x, y, z)){
				vertices[idx] = CreateVertex3(x, y, z);
				idx++;
			}

			int v1, v2, v3;
			idx = 0;
			while(path->GetFace3D(idx, v1, v2, v3)){
				faces[idx] = CreateTriangle3(v3, v2, v1);
				idx++;
			}

			mesh->SetGeometry(vertices, faces);

			sLib3MFTriangleProperties col = m_parse->GetColour(path->GetLayer()->Name);
			for(Lib3MF_uint32 i=0; i<mesh->GetTriangleCount(); i++){
				mesh->SetTriangleProperties(i, col);
			}
			mesh->SetObjectLevelProperty(col.m_ResourceID, col.m_PropertyIDs[0]);

			m_component->AddComponent(mesh.get(), m_wrapper->GetIdentityTransform());

		}
	}
}

void GDSObject_3mf::OutputPolygons()
{
	if(!m_polygons.empty()){
		GDSPolygon *polygon;

		for(unsigned long i=0; i<m_polygons.size(); i++){
			polygon = m_polygons[i];

			std::vector<sLib3MFPosition> vertices_3mf;
			std::vector<sLib3MFTriangle> triangles_3mf;

			std::vector<GDSVertex> vertices = polygon->GetVertices();
			std::vector<GDSTriangle> triangles = polygon->GetTriangles();

			for(int j=0; j<vertices.size(); j++){
				vertices_3mf.push_back(CreateVertex(vertices[j]));
			}

			for(int j=0; j<triangles.size(); j++){
				triangles_3mf.push_back(CreateTriangle(triangles[j]));
			}

			PMeshObject mesh = m_model->AddMeshObject();
			mesh->SetName(m_name);

			mesh->SetGeometry(vertices_3mf, triangles_3mf);

			sLib3MFTriangleProperties col = m_parse->GetColour(polygon->GetLayer()->Name);
			for(Lib3MF_uint32 i=0; i<mesh->GetTriangleCount(); i++){
				mesh->SetTriangleProperties(i, col);
			}
			mesh->SetObjectLevelProperty(col.m_ResourceID, col.m_PropertyIDs[0]);

			m_component->AddComponent(mesh.get(), m_wrapper->GetIdentityTransform());
		}
	}
}

void GDSObject_3mf::OutputTexts()
{
}

void GDSObject_3mf::OutputSRefs()
{
	for(unsigned int i = 0; i < m_srefs.size(); i++){
		ASRefElement *sref = m_srefs[i];

		GDSObject_3mf *oref = static_cast<GDSObject_3mf *>(m_parse->GetObjectRef(sref->name));

		m_component->AddComponent(oref->GetComponent().get(), CreateTransformMatrix(sref->x1, sref->y1, sref->rotate.y, sref->flipped, sref->mag));
	}
}

void GDSObject_3mf::OutputARefs()
{
	for(unsigned int i = 0; i < m_arefs.size(); i++){
		ASRefElement *aref = m_arefs[i];

		float dx = 0.0, dy = 0.0;
		int columns, rows;

		switch((int)round(aref->rotate.y)){
			case 0:
			case 360:
			case 180:
			case -180:
				dx = (float)(aref->x2 - aref->x1) / (float)aref->columns;
				dy = (float)(aref->y3 - aref->y1) / (float)aref->rows;
				columns = aref->columns;
				rows = aref->rows;
				break;

			case 90:
			case -270:
			case 270:
			case -90:
				dx = (float)(aref->x3 - aref->x1) / (float)aref->rows;
				dy = (float)(aref->y2 - aref->y1) / (float)aref->columns;
				columns = aref->rows;
				rows = aref->columns;
				break;
		}

		GDSObject_3mf *oref = static_cast<GDSObject_3mf *>(m_parse->GetObjectRef(aref->name));

		for(unsigned int c=0; c<columns; c++){
			for(unsigned int r=0; r<rows; r++){
				m_component->AddComponent(oref->GetComponent().get(), CreateTransformMatrix(aref->x1+dx*c, aref->y1+dy*r, aref->rotate.y, aref->flipped, aref->mag));
			}
		}
	}
}


void GDSObject_3mf::Output()
{
	if(!m_isoutput){
		OutputPolygons();
		OutputPaths();
		OutputSRefs();
		OutputTexts();
		OutputARefs();
	}
	m_isoutput = true;
}

Lib3MF::PComponentsObject GDSObject_3mf::GetComponent()
{
	return m_component;
}
