/*
 * File: gdsobject_pov.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the POV-RAY output specific implementation of the GDSObject class.
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

#include "gds_globals.h" //FIXME - this should be removed
#include "gdsobject_pov.h"
#include "gds2pov.h"

GDSObject_pov::GDSObject_pov(std::string Name) : GDSObject(Name){
}

GDSObject_pov::~GDSObject_pov()
{
}

void GDSObject_pov::OutputPathToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!PathItems.empty()){
		float angleX, angleY;

		class GDSPath *path;

		for(unsigned long i=0; i<PathItems.size(); i++){
			path = PathItems[i];

			if(path->GetWidth()){
				fprintf(fptr, "mesh2 { vertex_vectors { %d", 8*(path->GetPoints()-1));

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
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_j + PathWidth * angleX + extn_x,
						YCoords_j + PathWidth * angleY - extn_y,
						-path->GetHeight() - path->GetThickness()
						);
					// 2
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_j - PathWidth * angleX + extn_x,
						YCoords_j - PathWidth * angleY - extn_y,
						-path->GetHeight() - path->GetThickness()
						);
					// 3
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_j + PathWidth * angleX + extn_x,
						YCoords_j + PathWidth * angleY - extn_y,
						-path->GetHeight()
						);
					// 4
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_j - PathWidth * angleX + extn_x,
						YCoords_j - PathWidth * angleY - extn_y,
						-path->GetHeight()
						);

					// 5
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_jpone + PathWidth * angleX - extn_x,
						YCoords_jpone + PathWidth * angleY - extn_y,
						-path->GetHeight() - path->GetThickness()
						);

					// 6
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_jpone - PathWidth * angleX - extn_x,
						YCoords_jpone - PathWidth * angleY - extn_y,
						-path->GetHeight() - path->GetThickness()
						);

					// 7
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_jpone + PathWidth * angleX - extn_x,
						YCoords_jpone + PathWidth * angleY - extn_y,
						-path->GetHeight()
						);

					// 8
					fprintf(fptr, ",<%.2f,%.2f,%.2f>", 
						XCoords_jpone - PathWidth * angleX - extn_x,
						YCoords_jpone - PathWidth * angleY - extn_y,
						-path->GetHeight()
						);
				}
				unsigned int PathPoints = path->GetPoints();
				fprintf(fptr, "} face_indices { %d", 12*(PathPoints-1));
				for(unsigned int j=0; j<PathPoints-1; j++){
					// print ,faces now
					//int vertexindex[36] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 0, 1, 5, 0, 4, 5, 2, 3, 6, 3, 6, 7, 1, 3, 7, 1, 5, 7, 0, 2, 4, 2, 4, 6};
					fprintf(fptr, ",<%d,%d,%d>", 0+8*j, 1+8*j, 2+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 1+8*j, 2+8*j, 3+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 4+8*j, 5+8*j, 6+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 5+8*j, 6+8*j, 7+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 0+8*j, 1+8*j, 5+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 0+8*j, 4+8*j, 5+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 2+8*j, 3+8*j, 6+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 3+8*j, 6+8*j, 7+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 1+8*j, 3+8*j, 7+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 1+8*j, 5+8*j, 7+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 0+8*j, 2+8*j, 4+8*j);
					fprintf(fptr, ",<%d,%d,%d>", 2+8*j, 4+8*j, 6+8*j);
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

void GDSObject_pov::OutputPolygonToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!PolygonItems.empty()){
		if(decompose){
			DecomposePOVPolygons(fptr);
		}else{
			class GDSPolygon *polygon;

			for(unsigned long i=0; i<PolygonItems.size(); i++){
				polygon = PolygonItems[i];

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

void GDSObject_pov::OutputTextToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(!TextItems.empty()){
		class GDSText *text;
		//for (vector<class GDSText>::const_iterator text=TextItems.begin(); text!=TextItems.end(); ++text){
		for (unsigned int i=0; i<TextItems.size(); i++){
			text = TextItems[i];
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

void GDSObject_pov::OutputSRefToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	for(unsigned int i = 0; i < FirstSRef.size(); i++){
		SRefElement *sref = FirstSRef[i];

		fprintf(fptr, "object{str_%s ", sref->Name.c_str());
		if(sref->Mag!=1.0){
			fprintf(fptr, "scale <%.2f,%.2f,1> ", sref->Mag, sref->Mag);
		}
		if(sref->Flipped){
			fprintf(fptr, "scale <1,-1,1> ");
		}
		fprintf(fptr, "translate <%.2f,%.2f,0> ", sref->X, sref->Y);
		if(sref->Rotate.Y){
			fprintf(fptr, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -sref->Rotate.Y, sref->X, sref->Y);
		}
		fprintf(fptr, "}\n");
	}
}

void GDSObject_pov::OutputARefToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	for(unsigned int i = 0; i < FirstARef.size(); i++){
		ARefElement *aref = FirstARef[i];

		float dx, dy;

		if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
			if(aref->Columns && aref->Rows && (aref->X3 - aref->X1) && (aref->Y2 - aref->Y1)){
				dx = (float)(aref->X3 - aref->X1) / (float)aref->Columns;
				dy = (float)(aref->Y2 - aref->Y1) / (float)aref->Rows;

				fprintf(fptr, "#declare dx = %.2f;\n", dx);
				fprintf(fptr, "#declare dy = %.2f;\n", dy);

				fprintf(fptr, "#declare colcount = 0;\n");
				fprintf(fptr, "#declare cols = %d;\n", aref->Columns);
				fprintf(fptr, "#declare rows = %d;\n", aref->Rows);
				fprintf(fptr, "#while (colcount < cols)\n");
				fprintf(fptr, "\t#declare rowcount = 0;");
				fprintf(fptr, "\t#while (rowcount < rows)\n");
				fprintf(fptr, "\t\tobject{str_%s ", aref->Name.c_str());
				if(aref->Mag!=1.0){
					fprintf(fptr, "scale <%.2f,%.2f,1> ", aref->Mag, aref->Mag);
				}
				if(aref->Flipped){
					fprintf(fptr, "scale <1,-1,1> ");
				}
				fprintf(fptr, "translate <%.2f+dx*colcount,%.2f+dy*rowcount,0>", aref->X1, aref->Y1);
				if(aref->Rotate.Y){
					fprintf(fptr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f+dx*colcount,%.2f+dy*rowcount,0>)", -aref->Rotate.Y, aref->X1, aref->Y1);
				}
				fprintf(fptr, "}\n");

				fprintf(fptr, "\t\t#declare rowcount = rowcount + 1;\n");
				fprintf(fptr, "\t#end\n");
				fprintf(fptr, "\t#declare colcount = colcount + 1;\n");
				fprintf(fptr, "#end\n");
			}
		}else{
			if(aref->Columns && aref->Rows && (aref->X2 - aref->X1) && (aref->Y3 - aref->Y1)){
				dx = (float)(aref->X2 - aref->X1) / (float)aref->Columns;
				dy = (float)(aref->Y3 - aref->Y1) / (float)aref->Rows;

				fprintf(fptr, "#declare dx = %.2f;\n", dx);
				fprintf(fptr, "#declare dy = %.2f;\n", dy);

				fprintf(fptr, "#declare colcount = 0;\n");
				fprintf(fptr, "#declare cols = %d;\n", aref->Columns);
				fprintf(fptr, "#declare rows = %d;\n", aref->Rows);
				fprintf(fptr, "#while (colcount < cols)\n");
				fprintf(fptr, "\t#declare rowcount = 0;");
				fprintf(fptr, "\t#while (rowcount < rows)\n");
				fprintf(fptr, "\t\tobject{str_%s ", aref->Name.c_str());
				if(aref->Flipped){
					fprintf(fptr, "scale <1,-1,1> ");
				}
				fprintf(fptr, "translate <%.2f+dx*colcount,%.2f+dy*rowcount,0>", aref->X1, aref->Y1);
				if(aref->Rotate.Y){
					fprintf(fptr, " Rotate_Around_Trans(<0,0,%.2f>,<%.2f+dx*colcount,%.2f+dy*rowcount,0>)", -aref->Rotate.Y, aref->X1, aref->Y1);
				}
				fprintf(fptr, "}\n");

				fprintf(fptr, "\t\t#declare rowcount = rowcount + 1;\n");
				fprintf(fptr, "\t#end\n");
				fprintf(fptr, "\t#declare colcount = colcount + 1;\n");
				fprintf(fptr, "#end\n");
			}
		}
	}
}


void GDSObject_pov::OutputToFile(FILE *fptr, class GDSObjects *Objects, std::string Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	if(fptr && !IsOutput){
		fprintf(fptr, "#declare str_%s = union {\n", Name.c_str());

		OutputPolygonToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputPathToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputSRefToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputTextToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		OutputARefToFile(fptr, Objects, Font, offx, offy, objectid, firstlayer);

		fprintf(fptr, "}\n");
	}
	IsOutput = true;
}

void GDSObject_pov::DecomposePOVPolygons(FILE *fptr)
{
	unsigned long faceindex;

	if(!PolygonItems.empty()){
		class GDSPolygon *polygon;

		faceindex = 0;

		for(unsigned long i=0; i<PolygonItems.size(); i++){
			polygon = PolygonItems[i];

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

			pA.X = polygon->GetXCoords(0)-polygon->GetXCoords(polygon->GetPoints()-2);
			pA.Y = polygon->GetYCoords(0)-polygon->GetYCoords(polygon->GetPoints()-2);
			pB.X = polygon->GetXCoords(1)-polygon->GetXCoords(0);
			pB.Y = polygon->GetYCoords(1)-polygon->GetYCoords(0);

			float theta1;
			float theta2;

			theta1 = atan2(pA.X, pA.Y);
			theta2 = atan2(pB.X, pB.Y);
			polygon->SetAngleCoords(0, theta1 - theta2);

			for(unsigned int j=1; j<polygon->GetPoints()-1; j++){
				pA.X = polygon->GetXCoords(j)-polygon->GetXCoords(j-1);
				pA.Y = polygon->GetYCoords(j)-polygon->GetYCoords(j-1);

				pB.X = polygon->GetXCoords(j+1)-polygon->GetXCoords(j);
				pB.Y = polygon->GetYCoords(j+1)-polygon->GetYCoords(j);

				theta1 = atan2(pA.X, pA.Y);
				theta2 = atan2(pB.X, pB.Y);

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

			printf("+ve %d, -ve %d\n\n", positives, negatives);
		}

	}
}

