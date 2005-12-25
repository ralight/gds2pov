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

GDSObject_svg::GDSObject_svg(char *Name) : GDSObject(Name){
}

GDSObject_svg::~GDSObject_svg()
{
}

void GDSObject_svg::SetInitialOffset(float ioffx, float ioffy)
{
	_ioffx = ioffx;
	_ioffy = ioffy;
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
					fprintf(fptr, "%.2fpx,%.2fpx ", 
						_scale*(_ioffx + XCoords_j + PathWidth * angleX + extn_x),
						_scale*(_ioffy + YCoords_j + PathWidth * angleY - extn_y)
						);
					// 5
					fprintf(fptr, "%.2fpx,%.2fpx ", 
						_scale*(_ioffx + XCoords_jpone + PathWidth * angleX - extn_x),
						_scale*(_ioffy + YCoords_jpone + PathWidth * angleY - extn_y)
						);
					// 6
					fprintf(fptr, "%.2fpx,%.2fpx ", 
						_scale*(_ioffx + XCoords_jpone - PathWidth * angleX - extn_x),
						_scale*(_ioffy + YCoords_jpone - PathWidth * angleY - extn_y)
						);
					// 2
					fprintf(fptr, "%.2fpx,%.2fpx ", 
						_scale*(_ioffx + XCoords_j - PathWidth * angleX + extn_x),
						_scale*(_ioffy + YCoords_j - PathWidth * angleY - extn_y)
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

			//fprintf(fptr, "<polygon fill=\"pink\" stroke=\"black\" stroke-width=\"1px\" points=\"");
			fprintf(fptr, "\t\t\t<polygon class=\"%s\" points=\"", polygon->GetLayer()->Name);
			for(unsigned int j=0; j<polygon->GetPoints(); j++){
				fprintf(fptr, "%.2fpx,%.2fpx ",_scale*(polygon->GetXCoords(j)+_ioffx), _scale*(polygon->GetYCoords(j)+_ioffy));
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
		class GDSText *text;
		//for (vector<class GDSText>::const_iterator text=TextItems.begin(); text!=TextItems.end(); ++text){
		for (unsigned int i=0; i<TextItems.size(); i++){
			text = TextItems[i];
			if(text->GetString()){
				if(Font){
					fprintf(fptr, "text{ttf \"%s\" \"%s\" 0.2, 0 ", Font, text->GetString());
				}else{
					fprintf(fptr, "text{ttf \"crystal.ttf\" \"%s\" 0.2, 0 ", text->GetString());
				}
				//fprintf(fptr, "texture{pigment{rgbf <%.2f,%.2f,%.2f,%.2f>}} ", text->Colour.R, text->Colour.G, text->Colour.B, text->Colour.F);
				fprintf(fptr, "texture{t%s}",text->GetLayer()->Name);
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
						htrans = -0.5*strlen(text->GetString());
						break;
					case 1:
						htrans = -0.25*strlen(text->GetString());
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

void GDSObject_svg::OutputSRefToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(FirstSRef){
		SRefElement dummysref;
		dummysref.Next = FirstSRef;

		SRefElement *sref = &dummysref;

		while(sref->Next){
			sref = sref->Next;

			fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", _scale*(_ioffx+sref->X), _scale*(_ioffy+sref->Y), sref->Name);
			/* FIXME 
			if(sref->Mag!=1.0){
				fprintf(fptr, " scale <%.2f,%.2f,1>", sref->Mag, sref->Mag);
			}
			if(sref->Flipped){
				fprintf(fptr, " scale <1,-1,1> ");
			}
			*/
			if(sref->Rotate.Y){
				fprintf(fptr, " transform=\"rotate(%.2f,%.2f,%.2f)\"", -sref->Rotate.Y, _scale*(_ioffx+sref->X), _scale*(_ioffy+sref->Y));
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

		float dx, dy;

		while(aref->Next){
			aref = aref->Next;
			if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
				if(aref->Columns && aref->Rows && (aref->X3 - aref->X1) && (aref->Y2 - aref->Y1)){
					dx = (float)(aref->X3 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y2 - aref->Y1) / (float)aref->Rows;

					for(i=0; i<aref->Columns; i++){
						for(j=0; j<aref->Rows; j++){
							fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", _scale*(_ioffx+aref->X1+dx*i), _scale*(_ioffy+aref->Y1+dy*j), aref->Name);
							if(aref->Rotate.Y){
								fprintf(fptr, " transform=\"rotate(%.2f,%.2f,%.2f)\"", -aref->Rotate.Y, _scale*(_ioffx+aref->X1), _scale*(_ioffy+aref->Y1));
							}
							fprintf(fptr, "/>\n");
						}
					}
				}
			}else{
				if(aref->Columns && aref->Rows && (aref->X2 - aref->X1) && (aref->Y3 - aref->Y1)){
					dx = (float)(aref->X2 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y3 - aref->Y1) / (float)aref->Rows;

					for(i=0; i<aref->Columns; i++){
						for(j=0; j<aref->Rows; j++){
							fprintf(fptr, "\t\t\t<use x=\"%.2f\" y=\"%.2f\" xlink:href=\"#%s\"", _scale*(_ioffx+aref->X1+dx*i), _scale*(_ioffy+aref->Y1+dy*j), aref->Name);
							if(aref->Rotate.Y){
								fprintf(fptr, " transform=\"rotate(%.2f,%.2f,%.2f)\"", -aref->Rotate.Y, _scale*(_ioffx+aref->X1), _scale*(_ioffy+aref->Y1));
							}
							fprintf(fptr, "/>\n");
						}
					}
				}
			}
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

