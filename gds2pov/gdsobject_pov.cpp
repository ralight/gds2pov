/*
 * File: gdsobject_pov.cpp
 * Author: Roger Light
 * Project: gds2x
 *
 * This is the POV-RAY output specific implementation of the GDSObject class.
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

#include <cstdio>
#include <cstring>
#include <cmath>

#include "gdsobject_pov.h"

GDSObject_pov::GDSObject_pov(std::string name, FILE *optr) : GDS2X::Object(name), m_decompose(false)
{
	m_optr = optr;
}


GDSObject_pov::GDSObject_pov(GDS2X::Object *object, FILE *optr) : m_decompose(false)
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


GDSObject_pov::~GDSObject_pov()
{
}

void GDSObject_pov::OutputPaths()
{
	if(!m_paths.empty()){
		GDS2X::Path *path;

		for(unsigned long i=0; i<m_paths.size(); i++){
			path = m_paths[i];

			if(path->GetWidth()){
				fprintf(m_optr, "mesh2 { vertex_vectors { %d", 4*(path->GetPoints()));

				float x, y, z;
				for(unsigned int idx=0; path->GetPoint3D(idx, x, y, z); idx++){
					fprintf(m_optr, ",<%.2f,%.2f,%.2f>", x, y, -z);
				}
				unsigned int PathPoints = path->GetPoints();
				unsigned int face_count = 4+8*(PathPoints-1);
				fprintf(m_optr, "} face_indices { %d", face_count);

				int v1, v2, v3;
				for(unsigned int idx=0; idx<face_count; idx++){
					path->GetFace3D(idx, v1, v2, v3);
					fprintf(m_optr, ",<%d,%d,%d>", v1, v2, v3);
				}
				fprintf(m_optr, "} ");
				//if(!path->Colour.Metal){
				//	fprintf(m_optr, "pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} ", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);
				//}else{
				//	fprintf(m_optr, "pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA} ", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);
				//}
				fprintf(m_optr, "texture{t%s}",path->GetLayer()->Name.c_str());
				fprintf(m_optr, "}\n");
			}
		}
	}
}

void GDSObject_pov::OutputPolygons()
{
	if(!m_polygons.empty()){
		if(m_decompose){
			DecomposePOVPolygons();
		}else{
			GDS2X::Polygon *polygon;

			for(unsigned long i=0; i<m_polygons.size(); i++){
				polygon = m_polygons[i];

				fprintf(m_optr, "prism{%.2f,%.2f,%d",polygon->GetHeight(), polygon->GetHeight()+polygon->GetThickness(), polygon->GetPoints());
				for(unsigned int j=0; j<polygon->GetPoints(); j++){
					fprintf(m_optr, ",<%.2f,%.2f>", polygon->GetXCoords(j), polygon->GetYCoords(j));
				}
				fprintf(m_optr, " rotate<-90,0,0> ");

				fprintf(m_optr, "texture{t%s}", polygon->GetLayer()->Name.c_str());
				fprintf(m_optr, "}\n");
			}
		}
	}
}

void GDSObject_pov::OutputTexts()
{
	if(!m_texts.empty()){
		GDS2X::Text *text;
		//for (vector<GDSText>::const_iterator text=m_texts.begin(); text!=m_texts.end(); ++text){
		for (unsigned int i=0; i<m_texts.size(); i++){
			text = m_texts[i];
			if(text->GetString().length() > 0){
				fprintf(m_optr, "text{ttf \"crystal.ttf\" \"%s\" 0.2, 0 ", text->GetString().c_str());
				//fprintf(m_optr, "texture{pigment{rgbf <%.2f,%.2f,%.2f,%.2f>}} ", text->Colour.R, text->Colour.G, text->Colour.B, text->Colour.F);
				fprintf(m_optr, "texture{t%s}",text->GetLayer()->Name.c_str());
				if(text->GetMag()!=1.0){
					fprintf(m_optr, "scale <%.2f,%.2f,1> ", text->GetMag(), text->GetMag());
				}
				if(text->GetFlipped()){
					fprintf(m_optr, "scale <1,-1,1> ");
				}
				fprintf(m_optr, "translate <%.2f,%.2f,%.2f> ", text->GetX(), text->GetY(), -text->GetZ());
				if(text->GetRY()){
					fprintf(m_optr, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,%.2f>)", -text->GetRY(), text->GetX(), text->GetY(), -text->GetZ());
				}
				float htrans = 0.0, vtrans = 0.0;
				switch(text->GetHJust()){
					case 0:
						htrans = -0.5*text->GetString().length();
						break;
					case 1:
						htrans = -0.25*text->GetString().length();
						break;
					case 2:
						htrans = 0;
						break;
				}
				switch(text->GetVJust()){
					case 0:
						vtrans = 0.0;
						break;
					case 1:
						vtrans = -0.5;
						break;
					case 2:
						vtrans = -1.0;
						break;
				}
				if(htrans || vtrans){
					if(text->GetRY()){
						fprintf(m_optr, "translate <%.2f,%.2f,0> ", vtrans, htrans);
					}else{
						fprintf(m_optr, "translate <%.2f,%.2f,0> ", htrans, vtrans);
					}
				}
				fprintf(m_optr, "}\n");
			}
		}
	}
}

void GDSObject_pov::OutputSRefs()
{
	for(unsigned int i = 0; i < m_srefs.size(); i++){
		GDS2X::ASRefElement *sref = m_srefs[i];

		fprintf(m_optr, "object{str_%s ", sref->name.c_str());
		if(sref->mag!=1.0){
			fprintf(m_optr, "scale <%.2f,%.2f,1> ", sref->mag, sref->mag);
		}
		if(sref->flipped){
			fprintf(m_optr, "scale <1,-1,1> ");
		}
		fprintf(m_optr, "translate <%.2f,%.2f,0> ", sref->x1, sref->y1);
		if(sref->rotate.y){
			fprintf(m_optr, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -sref->rotate.y, sref->x1, sref->y1);
		}
		fprintf(m_optr, "}\n");
	}
}

void GDSObject_pov::OutputARefs()
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

		if(columns > 1){
			fprintf(m_optr, "#local dx = %.2f;\n", dx);
			fprintf(m_optr, "#local colcount = 0;\n");
			fprintf(m_optr, "#local cols = %d;\n", columns);
		}
		if(rows > 1){
			fprintf(m_optr, "#local dy = %.2f;\n", dy);
					fprintf(m_optr, "#local rows = %d;\n", rows);
		}

		if(columns > 1){
			fprintf(m_optr, "#while (colcount < cols)\n");
		}
		if(rows > 1){
			fprintf(m_optr, "\t#local rowcount = 0;\n");
			fprintf(m_optr, "\t#while (rowcount < rows)\n");
		}

		fprintf(m_optr, "\t\tobject{str_%s ", aref->name.c_str());
		if(aref->mag!=1.0){
			fprintf(m_optr, "scale <%.2f,%.2f,1> ", aref->mag, aref->mag);
		}
		if(aref->flipped){
			fprintf(m_optr, "scale <1,-1,1> ");
		}

		if(columns > 1 && rows > 1){
			fprintf(m_optr, "translate <%.2f+dx*colcount,%.2f+dy*rowcount,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(m_optr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f+dx*colcount,%.2f+dy*rowcount,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}else if(columns > 1){
			fprintf(m_optr, "translate <%.2f+dx*colcount,%.2f,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(m_optr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f+dx*colcount,%.2f,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}else if(rows > 1){
			fprintf(m_optr, "translate <%.2f,%.2f+dy*rowcount,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(m_optr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f+dy*rowcount,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}else{
			fprintf(m_optr, "translate <%.2f,%.2f,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(m_optr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}
		fprintf(m_optr, "}\n");

		if(rows > 1){
			fprintf(m_optr, "\t\t#local rowcount = rowcount + 1;\n");
			fprintf(m_optr, "\t#end\n");
		}
		if(columns > 1){
					fprintf(m_optr, "\t#local colcount = colcount + 1;\n");
			fprintf(m_optr, "#end\n");
		}
	}
}


void GDSObject_pov::Output()
{
	if(m_optr && !m_isoutput){
		fprintf(m_optr, "#declare str_%s = union {\n", m_name.c_str());

		OutputPolygons();
		OutputPaths();
		OutputSRefs();
		OutputTexts();
		OutputARefs();

		fprintf(m_optr, "}\n");
	}
	m_isoutput = true;
}

void GDSObject_pov::DecomposePOVPolygons()
{
	if(!m_polygons.empty()){
		GDS2X::Polygon *polygon;

		for(unsigned long i=0; i<m_polygons.size(); i++){
			polygon = m_polygons[i];

			/* Output vertices */
			fprintf(m_optr, "mesh2 { vertex_vectors { %d", 2*(polygon->GetPoints()-1));
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				fprintf(m_optr, ",<%.2f,%.2f,%.2f>",
					polygon->GetXCoords(j),
					polygon->GetYCoords(j),
					polygon->GetHeight() + polygon->GetThickness()
					);
			}
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){

				fprintf(m_optr, ",<%.2f,%.2f,%.2f>",
					polygon->GetXCoords(j),
					polygon->GetYCoords(j),
					polygon->GetHeight()
					);
			}

			/*
			 * Calculate angles between adjacent vertices.
			 * We do this to tell what "type" of polygon we are dealing with.
			 * Specifically, where any change of convex/concave takes place.
			 * Because the first and last points are identical we do not need
			 * to worry about the final vertex angle (it is already calculated
			 * in the 0th vertex).
			 */

			GDS2X::Point pA, pB;

			pA.x = polygon->GetXCoords(0)-polygon->GetXCoords(polygon->GetPoints()-2);
			pA.y = polygon->GetYCoords(0)-polygon->GetYCoords(polygon->GetPoints()-2);
			pB.x = polygon->GetXCoords(1)-polygon->GetXCoords(0);
			pB.y = polygon->GetYCoords(1)-polygon->GetYCoords(0);

			float theta1;
			float theta2;

			theta1 = atan2(pA.x, pA.y);
			theta2 = atan2(pB.x, pB.y);
			polygon->SetAngleCoords(0, theta1 - theta2);

			for(unsigned int j=1; j<polygon->GetPoints()-1; j++){
				pA.x = polygon->GetXCoords(j)-polygon->GetXCoords(j-1);
				pA.y = polygon->GetYCoords(j)-polygon->GetYCoords(j-1);

				pB.x = polygon->GetXCoords(j+1)-polygon->GetXCoords(j);
				pB.y = polygon->GetYCoords(j+1)-polygon->GetYCoords(j);

				theta1 = atan2(pA.x, pA.y);
				theta2 = atan2(pB.x, pB.y);

				polygon->SetAngleCoords(j, theta1 - theta2);
			}
			int positives = 0;
			int negatives = 0;
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				polygon->SetAngleCoords(j, (float)asin(sin((double)polygon->GetAngleCoords(j))));
				if(polygon->GetAngleCoords(j)>=0){
					positives++;
				}else{
					negatives++;
				}
			}

			int bendindex1;

			if(!positives || !negatives){
				fprintf(m_optr, "} face_indices { %d", 2*(polygon->GetPoints()-3) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=1; j<polygon->GetPoints()-2; j++){
					fprintf(m_optr, ",<%d,%d,%d>",0,j,j+1);
					fprintf(m_optr, ",<%d,%d,%d>",polygon->GetPoints()-1,j+polygon->GetPoints()-1,j+polygon->GetPoints()-1+1);
				}
			}else if(positives==1 && negatives>1){
				bendindex1 = -1;
				fprintf(m_optr, "} face_indices { %d", 2*(polygon->GetPoints()-2) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if(polygon->GetAngleCoords(j)>=0){
						bendindex1 = (int)j;
						break;
					}
				}
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if((int)j!=bendindex1){
						fprintf(m_optr, ",<%d,%d,%d>", bendindex1, j, j+1);
						fprintf(m_optr, ",<%d,%d,%d>", bendindex1+polygon->GetPoints()-1, j+polygon->GetPoints()-1, (j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j+1:j+polygon->GetPoints());
					}
				}
			}else if(negatives==1 && positives>1){
				bendindex1 = -1;
				fprintf(m_optr, "} face_indices { %d", 2*(polygon->GetPoints()-2) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if(polygon->GetAngleCoords(j)<0){
						bendindex1 = j;
						break;
					}
				}
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if((int)j!=bendindex1){
						fprintf(m_optr, ",<%d,%d,%d>", bendindex1, j, j+1);
						fprintf(m_optr, ",<%d,%d,%d>", bendindex1+polygon->GetPoints()-1, j+polygon->GetPoints()-1, (j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j+1:j+polygon->GetPoints());
					}
				}
			/*}else if(negatives==2 && positives>2){
				bendindex1 = -1;
				bendindex2 = -1;

				fprintf(m_optr, "} face_indices{%d", 2*(polygon->GetPoints()-2) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if(polygon->GetAngleCoords(j)<0 && bendindex1 == -1){
						bendindex1 = j;
					}else if(polygon->GetAngleCoords(j)<0){
						bendindex2 = j;
						break;
					}
				}
				for(unsigned int j=bendindex1; j<=bendindex2; j++){
					fprintf(m_optr, "<%d,%d,%d>",);
				}
				for(unsigned int j=0; j<bendindex1; j++){
					fprintf(m_optr, ",<%d,%d,%d>",);
				}
				*/
			}else{
				fprintf(m_optr, "} face_indices { %d", 2*(polygon->GetPoints()-1));
			}

			/* Always output the vertical faces regardless of whether we fill in the horizontal faces or not */
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				fprintf(m_optr, ",<%d,%d,%d>",j,j+polygon->GetPoints()-1,(j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j:j+polygon->GetPoints());
				fprintf(m_optr, ",<%d,%d,%d>",j,j+1,(j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j:j+polygon->GetPoints());
			}
			fprintf(m_optr,"}");
			fprintf(m_optr, "texture{t%s}", polygon->GetLayer()->Name.c_str());
			fprintf(m_optr, "}\n");

			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				if(polygon->GetAngleCoords(j)>=0){
					fprintf(m_optr,"text{ttf \"crystal.ttf\" \"%d+\" 0.2, 0 ", j);
				}else{
					fprintf(m_optr,"text{ttf \"crystal.ttf\" \"%d-\" 0.2, 0 ", j);
				}
				fprintf(m_optr, " scale <1.5,1.5,1.5>");
				fprintf(m_optr, " translate <%.2f,%.2f,%.2f> texture{pigment{rgb <1,1,1>}}}\n", \
						polygon->GetXCoords(j), \
						polygon->GetYCoords(j), polygon->GetHeight() - 1);
			}
		}
	}
}


void GDSObject_pov::Decompose(bool value)
{
	m_decompose = value;
}
