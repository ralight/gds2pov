/*
 * File: gdsobject_svg.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the SVG output specific implementation of the GDSObject class.
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
#include <cstdlib>

#include "gdsobject_svg.h"

#ifndef M_PI
#  define M_PI 3.141592653589793
#endif


GDSObject_svg::GDSObject_svg(std::string name) : GDSObject(name){
}

GDSObject_svg::GDSObject_svg(GDSObject *object)
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


GDSObject_svg::~GDSObject_svg()
{
}

void GDSObject_svg::SetScale(float scale)
{
	m_scale = scale;
}

void GDSObject_svg::OutputPathToFile(FILE *fptr, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(!m_paths.empty()){
		GDSPath *path;

		for(unsigned long i=0; i<m_paths.size(); i++){
			path = m_paths[i];

			if(path && path->GetWidth()){
				fprintf(fptr, "\t\t\t<path class=\"s%s\" style=\"stroke-width:%f;", path->GetLayer()->Name.c_str(), path->GetWidth()*m_scale*2);
				switch(path->GetType()){
					case 0:
						break;
					case 1:
						fprintf(fptr, "stroke-linecap:round;");
						break;
					case 2:
						fprintf(fptr, "stroke-linecap:square;");
						break;
					case 4:
						printf("WARNING: Unsupported path type 4.\n");
						break;
				}
				fprintf(fptr, "\" d=\"");

				float x, y;
				char cmd = 'M';
				for(unsigned int idx=0; path->GetPointCentre(idx, x, y); idx++){
					fprintf(fptr, "%c %f %f ", cmd, m_scale*x, m_scale*(this->GetHeight()-y));
					cmd = 'L';
				}
				fprintf(fptr, "\"/>\n");
			}
		}
	}
}

void GDSObject_svg::OutputPolygonToFile(FILE *fptr, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(!m_polygons.empty()){
		GDSPolygon *polygon;

		for(unsigned long i=0; i<m_polygons.size(); i++){
			polygon = m_polygons[i];

			fprintf(fptr, "\t\t\t<polygon class=\"%s\" points=\"", polygon->GetLayer()->Name.c_str());
			for(unsigned int j=0; j<polygon->GetPoints(); j++){
				fprintf(fptr, "%.2f,%.2f ",m_scale*(polygon->GetXCoords(j)), m_scale*(this->GetHeight() - polygon->GetYCoords(j)));
			}
			fprintf(fptr, "\"/>\n");
		}
	}
}

void GDSObject_svg::OutputTextToFile(FILE *fptr, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(!m_texts.empty()){
		std::string str;
		GDSText *text;
		float angle;
		//for (vector<GDSText>::const_iterator text=m_texts.begin(); text!=m_texts.end(); ++text){
		for (unsigned int i=0; i<m_texts.size(); i++){
			text = m_texts[i];
			if(text && text->GetString().length() > 0){
				str = text->GetString();
				angle = 180.0/M_PI*asin(sin(M_PI/180*text->GetRY()));
				/* FIXME if(Font){
				fprintf(fptr, "text{ttf \"%s\" \"%s\" 0.2, 0 ", Font, text->GetString());
				}else{
					fprintf(fptr, "text{ttf \"crystal.ttf\" \"%s\" 0.2, 0 ", text->GetString());
				}*/
				//fprintf(fptr, "texture{pigment{rgbf <%.2f,%.2f,%.2f,%.2f>}} ", text->Colour.R, text->Colour.G, text->Colour.B, text->Colour.F);
				fprintf(fptr, "\t\t\t<text font-family=\"sans-serif\" font-size=\"55\" x=\"%.2f\" y=\"%.2f\" class=\"%s\"",m_scale*(text->GetX()), m_scale*(this->GetHeight() - text->GetY()), text->GetLayer()->Name.c_str());
				float htrans = 0.0, vtrans = 0.0;
				switch(text->GetHJust()){
					case 0:
						htrans = -0.5f*str.length();
						break;
					case 1:
							htrans = -0.25f*str.length();
					break;
						case 2:
						htrans = 0.0f;
						break;
				}
				switch(text->GetVJust()){
					case 2:
						vtrans = 0.0f;
						break;
					case 1:
						vtrans = 0.5f;
						break;
					case 0:
						vtrans = 1.0f;
						break;
				}
				if(text->GetMag()!=1.0 || text->GetFlipped() || fabs(angle) > 0.0f || htrans < 0.0 || vtrans < 0.0){
					fprintf(fptr, " transform=\"");
					/* FIXME if(text->GetMag()!=1.0){
						fprintf(fptr, "scale <%.2f,%.2f,1> ", text->GetMag(), text->GetMag());
					} */
					/* FIXME if(text->GetFlipped()){
						fprintf(fptr, "scale <1,-1,1> ");
					} */
					if(fabs(angle) > 0.0f){
						fprintf(fptr, "rotate(%.2f,%.2f,%.2f)", angle, m_scale*text->GetX(), m_scale*(this->GetHeight() - text->GetY()));
					}
					if(htrans < 0.0 || vtrans < 0.0){
						fprintf(fptr, " translate(%.2f,%.2f)", 55.0*htrans, 55.0*vtrans);
					}
					fprintf(fptr, "\"");
				}
				/* Remove forbidden characters
				 * They should be escaped really, but that is for later
				 * FIXME!
				 */
				for(unsigned int j = 0; j < str.length(); j++){
					if(str[j] == '&' || str[j] == '<' || str[j] == '>'){
						str[j] = '_';
					}
				}

				fprintf(fptr, ">%s</text>\n", str.c_str());
			}
		}
	}
}

void GDSObject_svg::OutputSRefToFile(FILE *fptr, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	float height, x, angle;

	for(unsigned int i = 0; i < m_srefs.size(); i++){
		ASRefElement *sref = m_srefs[i];

		GDSObject *obj = sref->object;
		if(obj){
			height = obj->GetHeight();
		}else{
			height = 0.0f;
		}

		x = sref->x1;
		angle = 180/M_PI*asin(sin(sref->rotate.y*M_PI/180));

		if(sref->flipped){
			x = -x;
		}
		fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", m_scale*x, m_scale*(this->GetHeight() - sref->y1 - height), sref->name.c_str());

		if(sref->mag!=1.0 || sref->flipped || fabs(angle) > 0.0f){
			fprintf(fptr, " transform=\"");
			/* FIXME - mag isn't tested */
			/* if(sref->mag!=1.0){
				fprintf(fptr, " scale(%.2f)", sref->mag, sref->mag);
			} */
			if(sref->flipped){
				fprintf(fptr, " scale(-1,1)");
			}
			if(fabs(angle) > 0.0f){
				fprintf(fptr, " rotate(%.2f,%.2f,%.2f)", angle, m_scale*x, m_scale*(this->GetHeight() - sref->y1));
			}
			fprintf(fptr, "\"");
		}
		fprintf(fptr, "/>\n");
	}
}

void GDSObject_svg::OutputARefToFile(FILE *fptr, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	for(unsigned int i = 0; i < m_arefs.size(); i++){
		ASRefElement *aref = m_arefs[i];
		float x, y, angle;
		float height;
		float dx, dy;

		GDSObject *obj = aref->object;
		if(obj){
			height = obj->GetHeight();
		}else{
			height = 0.0f;
		}

		fprintf(fptr, "<!-- ARef Start -->\n");
		if(aref->columns && aref->rows){
			x = aref->x1;
			y = aref->y1;
			if(aref->rotate.y == 90.0 || aref->rotate.y == -90.0){
				dy = fabs((float)(aref->x3 - aref->x1) / (float)aref->columns);
				dx = fabs((float)(aref->y2 - aref->y1) / (float)aref->rows);
				/* FIXME - untested */
				if(aref->flipped){
					x = -x;
				}
			}else{
				dx = fabs((float)(aref->x2 - aref->x1) / (float)aref->columns);
				dy = fabs((float)(aref->y3 - aref->y1) / (float)aref->rows);
				if(aref->flipped){
					x = -x;
				}

			}

			angle = 180/M_PI*asin(sin(aref->rotate.y*M_PI/180));

			for(int j=0; j<aref->columns; j++){
				for(int k=0; k<aref->rows; k++){
					fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", m_scale*(x + dx * j), m_scale*(this->GetHeight() - (y + dy * k) - height), aref->name.c_str());
					if(aref->mag!=1.0f || aref->flipped || fabs(angle)>0.0f){
						fprintf(fptr, " transform=\"");
						/* FIXME - mag isn't tested */
						/* if(sref->mag!=1.0){
							fprintf(fptr, " scale(%.2f)", sref->mag, sref->mag);
						} */
						if(aref->flipped){
							fprintf(fptr, " scale(-1,1)");
						}
						if(fabs(angle)>0.0f){
							fprintf(fptr, " rotate(%.2f,%.2f,%.2f)", angle, m_scale*x, m_scale*(this->GetHeight() - aref->y1));
						}
						fprintf(fptr, "\"");
					}
					fprintf(fptr, "/>\n");
				}
			}
		}
		fprintf(fptr, "<!-- ARef End -->\n");
	}
}


void GDSObject_svg::OutputToFile(FILE *fptr, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	if(fptr && !m_isoutput){
		fprintf(fptr, "\t\t<symbol id=\"%s\" overflow=\"visible\">\n", m_name.c_str());

		OutputPolygonToFile(fptr, offx, offy, objectid, firstlayer);
		OutputPathToFile(fptr, offx, offy, objectid, firstlayer);
		OutputSRefToFile(fptr, offx, offy, objectid, firstlayer);
		OutputTextToFile(fptr, offx, offy, objectid, firstlayer);
		OutputARefToFile(fptr, offx, offy, objectid, firstlayer);

		fprintf(fptr, "\t\t</symbol>\n");
	}
	m_isoutput = true;
}
