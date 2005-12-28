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

#include <stdio.h>
#include <math.h>

#include "gds_globals.h" //FIXME - this should be removed
#include "gdsobject_svg.h"
#include "gds2svg.h"
#include "gdsobjects.h"

GDSObject_svg::GDSObject_svg(char *Name) : GDSObject(Name){
}

GDSObject_svg::~GDSObject_svg()
{
}

void GDSObject_svg::SetScale(float scale)
{
	_scale = scale;
}

void GDSObject_svg::OutputPathToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!PathItems.empty()){
		float angleX, angleY;

		class GDSPath *path;

		for(unsigned long i=0; i<PathItems.size(); i++){
			path = PathItems[i];

			if(path->GetWidth()){

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
					fprintf(fptr, "\t\t\t<polygon class=\"%s\" points=\"", path->GetLayer()->Name);
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
						_scale*(XCoords_j + PathWidth * angleX + extn_x),
						_scale*(YCoords_j + PathWidth * angleY - extn_y)
						);
					// 5
					fprintf(fptr, "%.2f,%.2f ", 
						_scale*(XCoords_jpone + PathWidth * angleX - extn_x),
						_scale*(YCoords_jpone + PathWidth * angleY - extn_y)
						);
					// 6
					fprintf(fptr, "%.2f,%.2f ", 
						_scale*(XCoords_jpone - PathWidth * angleX - extn_x),
						_scale*(YCoords_jpone - PathWidth * angleY - extn_y)
						);
					// 2
					fprintf(fptr, "%.2f,%.2f ", 
						_scale*(XCoords_j - PathWidth * angleX + extn_x),
						_scale*(YCoords_j - PathWidth * angleY - extn_y)
						);
					fprintf(fptr, "\"/>\n");
				}
			}
		}
	}
}

void GDSObject_svg::OutputPolygonToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!PolygonItems.empty()){
		class GDSPolygon *polygon;

		for(unsigned long i=0; i<PolygonItems.size(); i++){
			polygon = PolygonItems[i];

			//fprintf(fptr, "<polygon fill=\"pink\" stroke=\"black\" stroke-width=\"1\" points=\"");
			fprintf(fptr, "\t\t\t<polygon class=\"%s\" points=\"", polygon->GetLayer()->Name);
			for(unsigned int j=0; j<polygon->GetPoints(); j++){
				fprintf(fptr, "%.2f,%.2f ",_scale*(polygon->GetXCoords(j)), _scale*(polygon->GetYCoords(j)));
			}
			fprintf(fptr, "\"");
			//fprintf(fptr, " rotate<-90,0,0> ");

			fprintf(fptr, "/>\n");
		}
	}
}

void GDSObject_svg::OutputTextToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!TextItems.empty()){
		char *str = NULL;
		class GDSText *text;
		//for (vector<class GDSText>::const_iterator text=TextItems.begin(); text!=TextItems.end(); ++text){
		for (unsigned int i=0; i<TextItems.size(); i++){
			text = TextItems[i];
			str = (char *)malloc(strlen(text->GetString())+1);
			if(str){
				strncpy(str, text->GetString(), strlen(text->GetString()));
				/* FIXME if(Font){
					fprintf(fptr, "text{ttf \"%s\" \"%s\" 0.2, 0 ", Font, text->GetString());
				}else{
					fprintf(fptr, "text{ttf \"crystal.ttf\" \"%s\" 0.2, 0 ", text->GetString());
				}*/
				//fprintf(fptr, "texture{pigment{rgbf <%.2f,%.2f,%.2f,%.2f>}} ", text->Colour.R, text->Colour.G, text->Colour.B, text->Colour.F);
				fprintf(fptr, "\t\t\t<g><text font-family=\"sans-serif\" font-size=\"55\" x=\"%.2f\" y=\"%.2f\" class=\"%s\"",_scale*(text->GetX()), _scale*(text->GetY()), text->GetLayer()->Name);
				/* FIXME if(text->GetMag()!=1.0){
					fprintf(fptr, "scale <%.2f,%.2f,1> ", text->GetMag(), text->GetMag());
				} */
				/* FIXME if(text->GetFlipped()){
					fprintf(fptr, "scale <1,-1,1> ");
				} */
				if(text->GetRY()){
					fprintf(fptr, " transform=\"rotate(%.2f,%.2f,%.2f)\"", -text->GetRY(), text->GetX(), text->GetY());
				}
				float htrans = 0.0, vtrans = 0.0;
				switch(text->GetHJust()){
					case 0:
						htrans = -0.5*strlen(str);
						break;
					case 1:
						htrans = -0.25*strlen(str);
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
				/* FIXME if(htrans || vtrans){
					if(text->GetRY()){
						fprintf(fptr, "translate <%.2f,%.2f,0> ", vtrans, htrans);
					}else{
						fprintf(fptr, "translate <%.2f,%.2f,0> ", htrans, vtrans);
					}
				} */
				/* Remove forbidden characters 
				 * They should be escaped really, but that is for later
				 * FIXME!
				 */
				for(int j = 0; j < strlen(str); j++){
					if(str[j] == '&'){
						str[j] = '_';
					}
				}
				
				fprintf(fptr, ">%s</text></g>\n", str);
				free(str);
			}
			str = NULL;
		}
	}
}

void GDSObject_svg::OutputSRefToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(FirstSRef){
		SRefElement dummysref;
		struct _Boundary *bound;
		float width, height, x, angle;
		GDSObject *obj = NULL;
		dummysref.Next = FirstSRef;

		SRefElement *sref = &dummysref;

		while(sref->Next){
			sref = sref->Next;
			
			obj = Objects->GetObjectRef(sref->Name);
			if(obj){
				width = obj->GetWidth();
				height = obj->GetHeight();
			}else{
				width = 0.0f;
				height = 0.0f;
			}

			x = sref->X;
			angle = 180/M_PI*asin(sin(-sref->Rotate.Y*M_PI/180));

			if(sref->Flipped){
				x = -x;
			}
			fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", _scale*(x), _scale*(sref->Y), sref->Name);

			if(sref->Mag!=1.0 || sref->Flipped || angle){
				fprintf(fptr, " transform=\"");
				/* FIXME - Mag isn't tested */
				/* if(sref->Mag!=1.0){
					fprintf(fptr, " scale(%.2f)", sref->Mag, sref->Mag);
				} */
				if(sref->Flipped){
					fprintf(fptr, " scale(-1,1)");
				}
				if(angle){
					fprintf(fptr, " rotate(%.2f,%.2f,%.2f)", angle, _scale*(x), _scale*(sref->Y));
				}
				fprintf(fptr, "\"");
			}
			fprintf(fptr, "/>\n");
		}
	}
}

void GDSObject_svg::OutputARefToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(FirstARef){
		int i, j;
		ARefElement dummyaref;
		dummyaref.Next = FirstARef;
		ARefElement *aref = &dummyaref;
		float x, y, angle;

		float dx, dy;

		while(aref->Next){
			aref = aref->Next;
			fprintf(fptr, "<!-- ARef Start -->\n");
			if(aref->Columns && aref->Rows){
				x = aref->X1;
				y = aref->Y1;
				if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
					dy = fabs((float)(aref->X3 - aref->X1) / (float)aref->Columns);
					dx = fabs((float)(aref->Y2 - aref->Y1) / (float)aref->Rows);
					/* FIXME - untested */
					if(aref->Flipped){
						y = -y;
					}
				}else{
					dx = fabs((float)(aref->X2 - aref->X1) / (float)aref->Columns);
					dy = fabs((float)(aref->Y3 - aref->Y1) / (float)aref->Rows);
					if(aref->Flipped){
						x = -x;
					}

				}

				angle = 180/M_PI*asin(sin(-aref->Rotate.Y*M_PI/180));

				for(i=0; i<aref->Columns; i++){
					for(j=0; j<aref->Rows; j++){
						fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", _scale*(x+dx*i), _scale*(y+dy*j), aref->Name);
						if(aref->Mag!=1.0f || aref->Flipped || fabs(angle)>0.01f){
							fprintf(fptr, " transform=\"");
							/* FIXME - Mag isn't tested */
							/* if(sref->Mag!=1.0){
								fprintf(fptr, " scale(%.2f)", sref->Mag, sref->Mag);
							} */
							if(aref->Flipped){
								fprintf(fptr, " scale(-1,1)");
							}
							if(fabs(angle)>0.01f){
								fprintf(fptr, " rotate(%.2f,%.2f,%.2f)", angle, _scale*(aref->X1), _scale*(aref->Y1));
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
}


void GDSObject_svg::OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(fptr && !IsOutput){
		fprintf(fptr, "\t\t<symbol id=\"%s\" overflow=\"visible\">\n", Name);

		OutputPolygonToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputPathToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputSRefToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputTextToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputARefToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);

		fprintf(fptr, "\t\t</symbol>\n");
	}
	IsOutput = true;
}

