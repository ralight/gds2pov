/*
 * File: gdsobject_openscad.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the openscad output specific implementation of the GDSObject class.
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

#include "gdsobject_openscad.h"
#include "PolygonTriangulator.h"

GDSObject_openscad::GDSObject_openscad(std::string name, FILE *optr) : GDS2X::Object(name)
{
	m_optr = optr;
}


GDSObject_openscad::GDSObject_openscad(GDS2X::Object *object, FILE *optr)
{
	m_optr = optr;
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


GDSObject_openscad::~GDSObject_openscad()
{
}

void GDSObject_openscad::OutputPaths()
{
	if(!m_paths.empty()){
		GDS2X::Path *path;

		for(unsigned long i=0; i<m_paths.size(); i++){
			path = m_paths[i];

			char hex[20];
			path->GetLayer()->HexColour(hex, sizeof(hex));
			fprintf(m_optr, "color(\"#%s\"){\n", hex);

			fprintf(m_optr, "polyhedron(\n");
			{
				fprintf(m_optr, "\tpoints=[");

				float x, y, z;
				unsigned idx = 0;
				path->GetPoint3D(idx, x, y, z);

				fprintf(m_optr, "[%.4f,%.4f,%.4f]", x, y, z);
				idx = 1;
				while(path->GetPoint3D(idx, x, y, z)){
					fprintf(m_optr, ",[%.4f,%.4f,%.4f]", x, y, z);
					idx++;
				}
				fprintf(m_optr, "],\n");
			}

			{
				fprintf(m_optr, "\tfaces=[");
				{
					int v1, v2, v3;

					path->GetFace3D(0, v1, v2, v3);
					fprintf(m_optr, "[%d,%d,%d]", v1, v2, v3);
					unsigned int idx=1;
					while(path->GetFace3D(idx, v1, v2, v3)){
						fprintf(m_optr, ",[%d,%d,%d]", v1, v2, v3);
						idx++;
					}
				}
				fprintf(m_optr, "] ");
			}
			fprintf(m_optr, ");}\n");
		}
	}
}

void GDSObject_openscad::WritePolygonPoints(GDS2X::Polygon *polygon)
{
	std::vector<GDS2X::Vertex> vertices = polygon->GetVertices();

	fprintf(m_optr, "\tpoints=[");
	fprintf(m_optr, "[%.4f,%.4f,%.4f]", vertices[0].x, vertices[0].y, vertices[0].z);
	for(unsigned int j=1; j<vertices.size(); j++){
		fprintf(m_optr, ",[%.4f,%.4f,%.4f]", vertices[j].x, vertices[j].y, vertices[j].z);
	}
	fprintf(m_optr, "],\n");
}

void GDSObject_openscad::WritePolygonFaces(GDS2X::Polygon *polygon)
{
	std::vector<GDS2X::Triangle> triangles = polygon->GetTriangles();
	fprintf(m_optr, "\tfaces=[");
	fprintf(m_optr, "[%d,%d,%d]", triangles[0].v[0], triangles[0].v[1], triangles[0].v[2]);
	for(unsigned int j=1; j<triangles.size(); j++){
		fprintf(m_optr, ",[%d,%d,%d]", triangles[j].v[0], triangles[j].v[1], triangles[j].v[2]);
	}
	fprintf(m_optr,"]\n");
}


void GDSObject_openscad::OutputPolygons()
{
	if(!m_polygons.empty()){
		GDS2X::Polygon *polygon;

		for(unsigned long i=0; i<m_polygons.size(); i++){
			polygon = m_polygons[i];

			char hex[20];
			GDS2X::ProcessLayer *layer = polygon->GetLayer();
			if(layer){
				layer->HexColour(hex, sizeof(hex));
				fprintf(m_optr, "color(\"#%s\"){\n", hex);

				fprintf(m_optr, "polyhedron(\n");

				WritePolygonPoints(polygon);
				WritePolygonFaces(polygon);

				fprintf(m_optr, ");}\n");
			}else{
				printf("Warning: Polygon with unknown layer\n");
			}
		}
	}
}

void GDSObject_openscad::OutputTexts()
{
}

void GDSObject_openscad::OutputSRefs()
{
	for(unsigned int i = 0; i < m_srefs.size(); i++){
		GDS2X::ASRefElement *sref = m_srefs[i];

		fprintf(m_optr, "translate([%.4f,%.4f,0]){", sref->x1, sref->y1);

		if(sref->rotate.y){
			fprintf(m_optr, "rotate([0,0,%.4f]){", -sref->rotate.y);
		}
		if(sref->flipped){
			fprintf(m_optr, "scale([1,-1,1]){\n");
		}
		if(sref->mag!=1.0){
			fprintf(m_optr, "scale([%.4f,%.4f,1]){\n", sref->mag, sref->mag);
		}
		fprintf(m_optr, "mod_%s();", sref->name.c_str());
		if(sref->mag!=1.0){
			fprintf(m_optr, "}\n");
		}
		if(sref->flipped){
			fprintf(m_optr, "}\n");
		}
		if(sref->rotate.y){
			fprintf(m_optr, "}\n");
		}
		fprintf(m_optr, "}\n");
	}
}

void GDSObject_openscad::OutputARefs()
{
	for(unsigned int i = 0; i < m_arefs.size(); i++){
		GDS2X::ASRefElement *aref = m_arefs[i];

		float dx = 0.0, dy = 0.0;
		int columns = 0, rows = 0;

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

		fprintf(m_optr, "for(x=[0:%d]){\n", columns-1);
		fprintf(m_optr, "for(y=[0:%d]){\n", rows-1);

		fprintf(m_optr, "translate([%.4f+x*%.4f,%.4f+y*%.4f,0]){", aref->x1, dx, aref->y1, dy);

		if(aref->rotate.y){
			fprintf(m_optr, "rotate([0,0,%.4f]){", -aref->rotate.y);
		}
		if(aref->flipped){
			fprintf(m_optr, "scale([1,-1,1]){\n");
		}
		if(aref->mag!=1.0){
			fprintf(m_optr, "scale([%.4f,%.4f,1]){\n", aref->mag, aref->mag);
		}
		fprintf(m_optr, "mod_%s();", aref->name.c_str());
		if(aref->mag!=1.0){
			fprintf(m_optr, "}\n");
		}
		if(aref->flipped){
			fprintf(m_optr, "}\n");
		}
		if(aref->rotate.y){
			fprintf(m_optr, "}\n");
		}
		fprintf(m_optr, "}\n");

		fprintf(m_optr, "}\n");
		fprintf(m_optr, "}\n");
	}
}


void GDSObject_openscad::Output()
{
	if(m_optr && !m_isoutput){
		fprintf(m_optr, "module mod_%s(){\n", m_name.c_str());

		OutputPolygons();
		OutputPaths();
		OutputSRefs();
		OutputTexts();
		OutputARefs();

		fprintf(m_optr, "}\n");
	}
	m_isoutput = true;
}
