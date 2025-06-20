/*
 * File: gdsobject_pov.cpp
 * Author: Roger Light
 * Project: gdsto3d
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

GDSObject_pov::GDSObject_pov(std::string name) : GDSObject(name), m_decompose(false){
}


GDSObject_pov::GDSObject_pov(const GDSObject *object) : m_decompose(false)
{
	m_name = object->m_name;
	m_width = object->m_width;
	m_height = object->m_height;

	m_gotboundary = false;
	m_isoutput = false;

	m_paths = object->m_paths;
	m_texts = object->m_texts;
	m_polygons = object->m_polygons;

	m_srefs = object->m_srefs;
	m_arefs = object->m_arefs;
}


GDSObject_pov::~GDSObject_pov()
{
}

void GDSObject_pov::OutputPathToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(!m_paths.empty()){
		GDSPath *path;

		for(unsigned long i=0; i<m_paths.size(); i++){
			path = m_paths[i];

			if(path->GetWidth()){
				fprintf(fptr, "mesh2 { vertex_vectors { %d", 4*(path->GetPoints()));

				float BgnExtn;
				float EndExtn;

				switch(path->GetType()){
					case 1:
					case 2:
						BgnExtn = path->GetWidth(); /* Width has already been scaled to half */
						EndExtn = path->GetWidth();
							break;
					case 4:
						BgnExtn = path->GetBgnExtn();
						EndExtn = path->GetEndExtn();
						break;
					default:
						BgnExtn = 0.0;
						EndExtn = 0.0;
						break;
				}
				for(unsigned int j=0; j<path->GetPoints(); j++){
					double XCoords_j1, XCoords_j2, XCoords_j3;
					double YCoords_j1, YCoords_j2, YCoords_j3;
					float angle12, angle23;
					float angleX, angleY;
					float PathWidth = path->GetWidth();
					float extn_x, extn_y;

					XCoords_j2 = path->GetXCoords(j);
					YCoords_j2 = path->GetYCoords(j);
					if(j>0) {
						XCoords_j1 = path->GetXCoords(j-1);
						YCoords_j1 = path->GetYCoords(j-1);
						angle12 = atan2(XCoords_j1 - XCoords_j2, YCoords_j2 - YCoords_j1);
					}
					if(j+1<path->GetPoints()) {
						XCoords_j3 = path->GetXCoords(j+1);
						YCoords_j3 = path->GetYCoords(j+1);
						angle23 = atan2(XCoords_j2 - XCoords_j3, YCoords_j3 - YCoords_j2);
					}

					if(j==0) {
						extn_x = BgnExtn * sin(angle23);
						extn_y = BgnExtn * cos(angle23);
						angleX = cos(angle23);
						angleY = sin(angle23);
					}
					else if(j==path->GetPoints()-1) {
						extn_x = EndExtn * -sin(angle12);
						extn_y = EndExtn * -cos(angle12);
						angleX = cos(angle12);
						angleY = sin(angle12);
					}else{
						extn_x = 0.0;
						extn_y = 0.0;
						angleX = cos(angle12) - sin(angle12)*tan((angle23-angle12)*.5);
						angleY = sin(angle12) + cos(angle12)*tan((angle23-angle12)*.5);
					}

					// 1
					fprintf(fptr, ",<%.2f,%.2f,%.2f>",
						XCoords_j2 + PathWidth * angleX + extn_x,
						YCoords_j2 + PathWidth * angleY - extn_y,
						-path->GetHeight() - path->GetThickness()
						);
					// 2
					fprintf(fptr, ",<%.2f,%.2f,%.2f>",
						XCoords_j2 - PathWidth * angleX + extn_x,
						YCoords_j2 - PathWidth * angleY - extn_y,
						-path->GetHeight() - path->GetThickness()
						);
					// 3
					fprintf(fptr, ",<%.2f,%.2f,%.2f>",
						XCoords_j2 + PathWidth * angleX + extn_x,
						YCoords_j2 + PathWidth * angleY - extn_y,
						-path->GetHeight()
						);
					// 4
					fprintf(fptr, ",<%.2f,%.2f,%.2f>",
						XCoords_j2 - PathWidth * angleX + extn_x,
						YCoords_j2 - PathWidth * angleY - extn_y,
						-path->GetHeight()
						);
				}
				unsigned int PathPoints = path->GetPoints();
				fprintf(fptr, "} face_indices { %d", 4+8*(PathPoints-1));
				if(PathPoints>0) {
					fprintf(fptr, ",<%d,%d,%d>", 0, 1, 2);
					fprintf(fptr, ",<%d,%d,%d>", 1, 2, 3);
					fprintf(fptr, ",<%d,%d,%d>", 0+4*PathPoints-4, 1+4*PathPoints-4, 2+4*PathPoints-4);
					fprintf(fptr, ",<%d,%d,%d>", 1+4*PathPoints-4, 2+4*PathPoints-4, 3+4*PathPoints-4);
				}
				for(unsigned int j=0; j<PathPoints-1; j++){
					// print ,faces now
					//int vertexindex[36] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 0, 1, 5, 0, 4, 5, 2, 3, 6, 3, 6, 7, 1, 3, 7, 1, 5, 7, 0, 2, 4, 2, 4, 6};
					fprintf(fptr, ",<%d,%d,%d>", 0+4*j, 1+4*j, 5+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 0+4*j, 4+4*j, 5+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 2+4*j, 3+4*j, 6+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 3+4*j, 6+4*j, 7+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 1+4*j, 3+4*j, 7+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 1+4*j, 5+4*j, 7+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 0+4*j, 2+4*j, 4+4*j);
					fprintf(fptr, ",<%d,%d,%d>", 2+4*j, 4+4*j, 6+4*j);
				}
				fprintf(fptr, "} ");
				//if(!path->Colour.Metal){
				//	fprintf(fptr, "pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} ", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);
				//}else{
				//	fprintf(fptr, "pigment{rgbf <%.2f, %.2f, %.2f, %.2f>} finish{F_MetalA} ", path->Colour.R, path->Colour.G, path->Colour.B, path->Colour.F);
				//}
				fprintf(fptr, "texture{t%s}",path->GetLayer()->Name.c_str());
				fprintf(fptr, "}\n");
			}
		}
	}
}

void GDSObject_pov::OutputPolygonToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(!m_polygons.empty()){
		if(m_decompose){
			DecomposePOVPolygons(fptr);
		}else{
			GDSPolygon *polygon;

			for(unsigned long i=0; i<m_polygons.size(); i++){
				polygon = m_polygons[i];

				fprintf(fptr, "prism{%.2f,%.2f,%d",polygon->GetHeight(), polygon->GetHeight()+polygon->GetThickness(), polygon->GetPoints());
				for(unsigned int j=0; j<polygon->GetPoints(); j++){
					fprintf(fptr, ",<%.2f,%.2f>", polygon->GetXCoords(j), polygon->GetYCoords(j));
				}
				fprintf(fptr, " rotate<-90,0,0> ");

				fprintf(fptr, "texture{t%s}", polygon->GetLayer()->Name.c_str());
				fprintf(fptr, "}\n");
			}
		}
	}
}

void GDSObject_pov::OutputTextToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(!m_texts.empty()){
		GDSText *text;
		//for (vector<GDSText>::const_iterator text=m_texts.begin(); text!=m_texts.end(); ++text){
		for (unsigned int i=0; i<m_texts.size(); i++){
			text = m_texts[i];
			if(text->GetString().length() > 0){
				if(Font.length() > 0){
					fprintf(fptr, "text{ttf \"%s\" \"%s\" 0.2, 0 ", Font.c_str(), text->GetString().c_str());
				}else{
					fprintf(fptr, "text{ttf \"crystal.ttf\" \"%s\" 0.2, 0 ", text->GetString().c_str());
				}
				//fprintf(fptr, "texture{pigment{rgbf <%.2f,%.2f,%.2f,%.2f>}} ", text->Colour.R, text->Colour.G, text->Colour.B, text->Colour.F);
				fprintf(fptr, "texture{t%s}",text->GetLayer()->Name.c_str());
				if(text->GetMag()!=1.0){
					fprintf(fptr, "scale <%.2f,%.2f,1> ", text->GetMag(), text->GetMag());
				}
				if(text->GetFlipped()){
					fprintf(fptr, "scale <1,-1,1> ");
				}
				fprintf(fptr, "translate <%.2f,%.2f,%.2f> ", text->GetX(), text->GetY(), -text->GetZ());
				if(text->GetRY()){
					fprintf(fptr, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,%.2f>)", -text->GetRY(), text->GetX(), text->GetY(), -text->GetZ());
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
						fprintf(fptr, "translate <%.2f,%.2f,0> ", vtrans, htrans);
					}else{
						fprintf(fptr, "translate <%.2f,%.2f,0> ", htrans, vtrans);
					}
				}
				fprintf(fptr, "}\n");
			}
		}
	}
}

void GDSObject_pov::OutputSRefToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	for(unsigned int i = 0; i < m_srefs.size(); i++){
		ASRefElement *sref = m_srefs[i];

		fprintf(fptr, "object{str_%s ", sref->name.c_str());
		if(sref->mag!=1.0){
			fprintf(fptr, "scale <%.2f,%.2f,1> ", sref->mag, sref->mag);
		}
		if(sref->flipped){
			fprintf(fptr, "scale <1,-1,1> ");
		}
		fprintf(fptr, "translate <%.2f,%.2f,0> ", sref->x1, sref->y1);
		if(sref->rotate.y){
			fprintf(fptr, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -sref->rotate.y, sref->x1, sref->y1);
		}
		fprintf(fptr, "}\n");
	}
}

void GDSObject_pov::OutputARefToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
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

		if(columns > 1){
			fprintf(fptr, "#local dx = %.2f;\n", dx);
			fprintf(fptr, "#local colcount = 0;\n");
			fprintf(fptr, "#local cols = %d;\n", columns);
		}
		if(rows > 1){
			fprintf(fptr, "#local dy = %.2f;\n", dy);
					fprintf(fptr, "#local rows = %d;\n", rows);
		}

		if(columns > 1){
			fprintf(fptr, "#while (colcount < cols)\n");
		}
		if(rows > 1){
			fprintf(fptr, "\t#local rowcount = 0;\n");
			fprintf(fptr, "\t#while (rowcount < rows)\n");
		}

		fprintf(fptr, "\t\tobject{str_%s ", aref->name.c_str());
		if(aref->mag!=1.0){
			fprintf(fptr, "scale <%.2f,%.2f,1> ", aref->mag, aref->mag);
		}
		if(aref->flipped){
			fprintf(fptr, "scale <1,-1,1> ");
		}

		if(columns > 1 && rows > 1){
			fprintf(fptr, "translate <%.2f+dx*colcount,%.2f+dy*rowcount,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(fptr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f+dx*colcount,%.2f+dy*rowcount,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}else if(columns > 1){
			fprintf(fptr, "translate <%.2f+dx*colcount,%.2f,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(fptr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f+dx*colcount,%.2f,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}else if(rows > 1){
			fprintf(fptr, "translate <%.2f,%.2f+dy*rowcount,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(fptr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f+dy*rowcount,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}else{
			fprintf(fptr, "translate <%.2f,%.2f,0>", aref->x1, aref->y1);
			if(aref->rotate.y){
				fprintf(fptr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -aref->rotate.y, aref->x1, aref->y1);
			}
		}
		fprintf(fptr, "}\n");

		if(rows > 1){
			fprintf(fptr, "\t\t#local rowcount = rowcount + 1;\n");
			fprintf(fptr, "\t#end\n");
		}
		if(columns > 1){
					fprintf(fptr, "\t#local colcount = colcount + 1;\n");
			fprintf(fptr, "#end\n");
		}
	}
}


void GDSObject_pov::OutputToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(fptr && !m_isoutput){
		fprintf(fptr, "#declare str_%s = union {\n", m_name.c_str());

		OutputPolygonToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputPathToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputSRefToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputTextToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputARefToFile(fptr, Font, offx, offy, objectid, firstlayer);

		fprintf(fptr, "}\n");
	}
	m_isoutput = true;
}

void GDSObject_pov::DecomposePOVPolygons(FILE *fptr)
{
	if(!m_polygons.empty()){
		GDSPolygon *polygon;

		for(unsigned long i=0; i<m_polygons.size(); i++){
			polygon = m_polygons[i];

			/* Output vertices */
			fprintf(fptr, "mesh2 { vertex_vectors { %d", 2*(polygon->GetPoints()-1));
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				fprintf(fptr, ",<%.2f,%.2f,%.2f>",
					polygon->GetXCoords(j),
					polygon->GetYCoords(j),
					polygon->GetHeight() + polygon->GetThickness()
					);
			}
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){

				fprintf(fptr, ",<%.2f,%.2f,%.2f>",
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

			Point pA, pB;

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
				fprintf(fptr, "} face_indices { %d", 2*(polygon->GetPoints()-3) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=1; j<polygon->GetPoints()-2; j++){
					fprintf(fptr, ",<%d,%d,%d>",0,j,j+1);
					fprintf(fptr, ",<%d,%d,%d>",polygon->GetPoints()-1,j+polygon->GetPoints()-1,j+polygon->GetPoints()-1+1);
				}
			}else if(positives==1 && negatives>1){
				bendindex1 = -1;
				fprintf(fptr, "} face_indices { %d", 2*(polygon->GetPoints()-2) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if(polygon->GetAngleCoords(j)>=0){
						bendindex1 = (int)j;
						break;
					}
				}
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if((int)j!=bendindex1){
						fprintf(fptr, ",<%d,%d,%d>", bendindex1, j, j+1);
						fprintf(fptr, ",<%d,%d,%d>", bendindex1+polygon->GetPoints()-1, j+polygon->GetPoints()-1, (j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j+1:j+polygon->GetPoints());
					}
				}
			}else if(negatives==1 && positives>1){
				bendindex1 = -1;
				fprintf(fptr, "} face_indices { %d", 2*(polygon->GetPoints()-2) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if(polygon->GetAngleCoords(j)<0){
						bendindex1 = j;
						break;
					}
				}
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if((int)j!=bendindex1){
						fprintf(fptr, ",<%d,%d,%d>", bendindex1, j, j+1);
						fprintf(fptr, ",<%d,%d,%d>", bendindex1+polygon->GetPoints()-1, j+polygon->GetPoints()-1, (j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j+1:j+polygon->GetPoints());
					}
				}
			/*}else if(negatives==2 && positives>2){
				bendindex1 = -1;
				bendindex2 = -1;

				fprintf(fptr, "} face_indices{%d", 2*(polygon->GetPoints()-2) + 2*(polygon->GetPoints()-1));
				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					if(polygon->GetAngleCoords(j)<0 && bendindex1 == -1){
						bendindex1 = j;
					}else if(polygon->GetAngleCoords(j)<0){
						bendindex2 = j;
						break;
					}
				}
				for(unsigned int j=bendindex1; j<=bendindex2; j++){
					fprintf(fptr, "<%d,%d,%d>",);
				}
				for(unsigned int j=0; j<bendindex1; j++){
					fprintf(fptr, ",<%d,%d,%d>",);
				}
				*/
			}else{
				fprintf(fptr, "} face_indices { %d", 2*(polygon->GetPoints()-1));
			}

			/* Always output the vertical faces regardless of whether we fill in the horizontal faces or not */
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				fprintf(fptr, ",<%d,%d,%d>",j,j+polygon->GetPoints()-1,(j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j:j+polygon->GetPoints());
				fprintf(fptr, ",<%d,%d,%d>",j,j+1,(j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j:j+polygon->GetPoints());
			}
			fprintf(fptr,"}");
			fprintf(fptr, "texture{t%s}", polygon->GetLayer()->Name.c_str());
			fprintf(fptr, "}\n");

			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				if(polygon->GetAngleCoords(j)>=0){
					fprintf(fptr,"text{ttf \"crystal.ttf\" \"%d+\" 0.2, 0 ", j);
				}else{
					fprintf(fptr,"text{ttf \"crystal.ttf\" \"%d-\" 0.2, 0 ", j);
				}
				fprintf(fptr, " scale <1.5,1.5,1.5>");
				fprintf(fptr, " translate <%.2f,%.2f,%.2f> texture{pigment{rgb <1,1,1>}}}\n", \
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
