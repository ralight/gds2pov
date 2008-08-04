/*
 * File: gdsobject_svg.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the SVG output specific implementation of the GDSObject class.
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
#include <cstdlib>

#include "gds_globals.h" //FIXME - this should be removed
#include "gdsobject_svg.h"

GDSObject_svg::GDSObject_svg(std::string name) : GDSObject(name){
}

GDSObject_svg::~GDSObject_svg()
{
}

void GDSObject_svg::SetScale(float scale)
{
	m_scale = scale;
}

void GDSObject_svg::OutputPathToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!m_paths.empty()){
		float angleX, angleY;

		class GDSPath *path;

		for(unsigned long i=0; i<m_paths.size(); i++){
			path = m_paths[i];

			if(path && path->GetWidth()){

				float BgnExtn;
				float EndExtn;
				float extn_x, extn_y;

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
				for(unsigned int j=0; j<path->GetPoints()-1; j++){
					fprintf(fptr, "\t\t\t<polygon class=\"%s\" points=\"", path->GetLayer()->Name.c_str());
					float XCoords_j, XCoords_jpone;
					float YCoords_j, YCoords_jpone;
					float PathWidth = path->GetWidth();

					XCoords_j = path->GetXCoords(j);
					XCoords_jpone = path->GetXCoords(j+1);
					YCoords_j = path->GetYCoords(j);
					YCoords_jpone = path->GetYCoords(j+1);

					angleX = cos(atan2(XCoords_j - XCoords_jpone, YCoords_jpone - YCoords_j));
					angleY = sin(atan2(XCoords_j - XCoords_jpone, YCoords_jpone - YCoords_j));

					if(j==0 || j==path->GetPoints()-2){
						extn_x = EndExtn * angleY;
						extn_y = EndExtn * angleX;
					}else{
						extn_x = 0.0;
						extn_y = 0.0;
					}

					// 1
					fprintf(fptr, "%.2f,%.2f ", 
						m_scale*(XCoords_j + PathWidth * angleX + extn_x),
						m_scale*(this->GetHeight() - (YCoords_j + PathWidth * angleY - extn_y))
						);
					// 5
					fprintf(fptr, "%.2f,%.2f ", 
						m_scale*(XCoords_jpone + PathWidth * angleX - extn_x),
						m_scale*(this->GetHeight() - (YCoords_jpone + PathWidth * angleY - extn_y))
						);
					// 6
					fprintf(fptr, "%.2f,%.2f ", 
						m_scale*(XCoords_jpone - PathWidth * angleX - extn_x),
						m_scale*(this->GetHeight() - (YCoords_jpone - PathWidth * angleY - extn_y))
						);
					// 2
					fprintf(fptr, "%.2f,%.2f ", 
						m_scale*(XCoords_j - PathWidth * angleX + extn_x),
						m_scale*(this->GetHeight() - (YCoords_j - PathWidth * angleY - extn_y))
						);
					fprintf(fptr, "\"/>\n");
				}
			}
		}
	}
}

void GDSObject_svg::OutputPolygonToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!m_polygons.empty()){
		class GDSPolygon *polygon;

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

void GDSObject_svg::OutputTextToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!m_texts.empty()){
		std::string str;
		class GDSText *text;
		float angle;
		//for (vector<class GDSText>::const_iterator text=m_texts.begin(); text!=m_texts.end(); ++text){
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

void GDSObject_svg::OutputSRefToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
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

void GDSObject_svg::OutputARefToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
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


void GDSObject_svg::OutputToFile(FILE *fptr, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(fptr && !m_isoutput){
		fprintf(fptr, "\t\t<symbol id=\"%s\" overflow=\"visible\">\n", m_name.c_str());

		OutputPolygonToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputPathToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputSRefToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputTextToFile(fptr, Font, offx, offy, objectid, firstlayer);
		OutputARefToFile(fptr, Font, offx, offy, objectid, firstlayer);

		fprintf(fptr, "\t\t</symbol>\n");
	}
	m_isoutput = true;
}

