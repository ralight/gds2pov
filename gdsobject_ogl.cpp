#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include "gds_globals.h"
#include "gdsobject.h"
#include "gdsobjects.h"
#include "process_cfg.h"

typedef struct{
	float X;
	float Y;
	float Z;
} Point3D;

void GDSObject::OutputOGLVertices(FILE *optr, float offx, float offy, class GDSProcess *process)
{
	float angleX, angleY;
	float tempf;
	float BgnExtn;
	float EndExtn;
	float extn_x;
	float extn_y;
	Point3D points[8];
	struct ProcessLayer *layer;
	class GDSPath *path;

	if(!PathItems.empty() || !PolygonItems.empty()){

		if(PathItems.empty()){
			for(unsigned long i=0; i<PathItems.size(); i++){
				path = PathItems[i];

				layer = path->GetLayer();
				int plist[] = {7,4,5, 5,6,7, 2,1,0, 0,3,2, 6,5,1, 1,2,6, 4,7,3, 3,0,4, 6,2,3, 3,7,6, 4,0,1, 1,5,4};
				if(path->GetWidth() && layer){
#ifdef OGLVIEWER
					glPolygonMode(GL_FRONT_AND_BACK, render_mode);
					glColor3f(layer->Red, layer->Green, layer->Blue);
					glBegin(GL_TRIANGLES);
#else
					fprintf(optr, "\tglColor3f(%ff,%ff,%ff);\n", layer->Red, layer->Green, layer->Blue);
					fprintf(optr, "\tglBegin(GL_TRIANGLES);\n");
#endif

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

					float dx, dy;
					for(i=0; i<path->GetPoints()-1; i++){
						dx = path->GetXCoords(i) - path->GetXCoords(i+1);
						dy = path->GetYCoords(i+1) - path->GetYCoords(i);

						if(dx < 0.01){
							angleX = (dy < 0.0)? -1.0:1.0;
							angleY = 0.0;
						}else if(dy < 0.01){
							angleX = 0.0;
							angleY = (dx < 0.0)? -1.0:1.0;
						}else if(dx + dy < 0.01){
							if(dx < 0.0){
								angleX = 0.707107f;
								angleY = -0.707107f;
							}else{
								angleX = -0.707107f;
								angleY = 0.707107f;
							}
						}else if(dx - dy < 0.01){
							if(dx < 0.0){
								angleX = -0.707107f;
								angleY = -0.707107f;
							}else{
								angleX = 0.707107f;
								angleY = 0.707107f;
							}
						}else{
							angleX = cos(atan2(dx, dy));
							angleY = sin(atan2(dx, dy));
						}


						if(i==0 || i==path->GetPoints()-2){
							extn_x = EndExtn * angleY;
							extn_y = EndExtn * angleX;
						}else{
							extn_x = 0.0;
							extn_y = 0.0;
						}

						// 1
						points[0].X = path->GetXCoords(i) + path->GetWidth() * angleX + extn_x + offx;
						points[0].Y = path->GetYCoords(i) + path->GetWidth() * angleY - extn_y + offy;
						points[0].Z = path->GetHeight() + path->GetThickness();
						// 5
						points[4].X = points[0].X;
						points[4].Y = points[0].Y;
						points[4].Z = path->GetHeight();
						// 2
						points[1].X = path->GetXCoords(i) - path->GetWidth() * angleX + extn_x + offx;
						points[1].Y = path->GetYCoords(i) - path->GetWidth() * angleY - extn_y + offy;
						points[1].Z = path->GetHeight() + path->GetThickness();
						// 6
						points[5].X = points[1].X;
						points[5].Y = points[1].Y;
						points[5].Z = path->GetHeight();
						// 3 
						points[2].X = path->GetXCoords(i+1) - path->GetWidth() * angleX - extn_x + offx;
						points[2].Y = path->GetYCoords(i+1) - path->GetWidth() * angleY - extn_y + offy;
						points[2].Z = path->GetHeight() + path->GetThickness();
						// 7
						points[6].X = points[2].X;
						points[6].Y = points[2].Y;
						points[6].Z = path->GetHeight();
						// 4
						points[3].X = path->GetXCoords(i+1) + path->GetWidth() * angleX - extn_x + offx;
						points[3].Y = path->GetYCoords(i+1) + path->GetWidth() * angleY - extn_y + offy;
						points[3].Z = path->GetHeight() + path->GetThickness();
						// 8
						points[7].X = points[3].X;
						points[7].Y = points[3].Y;
						points[7].Z = path->GetHeight();

						for(unsigned int j=0; j<12; j++){
#ifdef OGLVIEWER
							glEdgeFlag(GL_TRUE);
							glVertex3f(points[plist[0+3*j]].X,points[plist[0+3*j]].Y, points[plist[0+3*j]].Z);
							glVertex3f(points[plist[1+3*j]].X,points[plist[1+3*j]].Y, points[plist[1+3*j]].Z);
							glEdgeFlag(GL_FALSE);
							glVertex3f(points[plist[2+3*j]].X,points[plist[2+3*j]].Y, points[plist[2+3*j]].Z);
#else
							fprintf(optr, "\tglEdgeFlag(GL_TRUE);\n");
							fprintf(optr, "\tglVertex3f(%ff,%ff,%ff);\n", points[plist[0+3*j]].X,points[plist[0+3*j]].Y, points[plist[0+3*j]].Z);
							fprintf(optr, "\tglVertex3f(%ff,%ff,%ff);\n", points[plist[1+3*j]].X,points[plist[1+3*j]].Y, points[plist[1+3*j]].Z);
							fprintf(optr, "\tglEdgeFlag(GL_FALSE);\n");
							fprintf(optr, "\tglVertex3f(%ff,%ff,%ff);\n", points[plist[2+3*j]].X,points[plist[2+3*j]].Y, points[plist[2+3*j]].Z);
#endif
						}
					}
#ifdef OGLVIEWER
					glEnd();
#else
					fprintf(optr, "\tglEnd();\n");
#endif
				}
			}
		}

		if(!PolygonItems.empty()){
			class GDSPolygon *polygon;

			for(unsigned long i=0; i<PolygonItems.size(); i++){
				polygon = PolygonItems[i];

				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					tempf = polygon->GetXCoords(j) + offx;
					tempf = polygon->GetYCoords(j) + offy;
					tempf = polygon->GetHeight() + polygon->GetThickness();
				}

				for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
					
					tempf = polygon->GetXCoords(j) + offx;
					tempf = polygon->GetYCoords(j) + offy;
					tempf = polygon->GetHeight();
				}
			}
		}
	}
}

void GDSObject::OutputOGLSRefs(FILE *optr, class GDSObjects *Objects, char *Font, float offx, float offy, class GDSProcess *process)
{
	GDSObject *obj;

	if(FirstSRef){
		SRefElement dummysref;
		dummysref.Next = FirstSRef;

		SRefElement *sref = &dummysref;

		while(sref->Next){
			sref = sref->Next;

			//obj = Objects->GetObject(sref->Name);
			obj = sref->object;
			if(obj){
				obj->OutputToOGL(optr, Objects, Font, offx+sref->X, offy+sref->Y, process);
			}
	//		if(sref->Mag!=1.0){
	//			fprintf(fptr, "scale <%.2f,%.2f,1> ", sref->Mag, sref->Mag);
	//		}
	//		if(sref->Flipped){
	//			fprintf(fptr, "scale <1,-1,1> ");
	//		}
	//		fprintf(fptr, "translate <%.2f,%.2f,0> ", sref->X, sref->Y);
	//		if(sref->Rotate.Y){
	//			fprintf(fptr, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -sref->Rotate.Y, sref->X, sref->Y);
	//		}
	//		fprintf(fptr, "}\n");
	
		}
	}
}

void GDSObject::OutputOGLARefs(FILE *optr, class GDSObjects *Objects, char *Font, float offx, float offy, class GDSProcess *process)
{
	GDSObject *obj;
	int i, j;

	if(FirstARef){
		ARefElement dummyaref;
		dummyaref.Next = FirstARef;
		ARefElement *aref = &dummyaref;

		float dx, dy;

		while(aref->Next){
			aref = aref->Next;
			//obj = Objects->GetObject(aref->Name);
			obj = aref->object;
			if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
				if(aref->Columns && aref->Rows && (aref->X3 - aref->X1) && (aref->Y2 - aref->Y1)){
					dx = (float)(aref->X3 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y2 - aref->Y1) / (float)aref->Rows;

					if(obj){
						for(i=0; i<aref->Rows; i++){
							for(j=0; j<aref->Columns; j++){
								obj->OutputToOGL(optr, Objects, Font, offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, process);
							}
						}
					}
				/*
					fprintf(fptr, "#declare dx = %.2f;\n", dx);
					fprintf(fptr, "#declare dy = %.2f;\n", dy);

					fprintf(fptr, "#declare colcount = 0;\n");
					fprintf(fptr, "#declare cols = %d;\n", aref->Columns);
					fprintf(fptr, "#declare rows = %d;\n", aref->Rows);
					fprintf(fptr, "#while (colcount < cols)\n");
					fprintf(fptr, "\t#declare rowcount = 0;");
					fprintf(fptr, "\t#while (rowcount < rows)\n");
					fprintf(fptr, "\t\tobject{str_%s ", aref->Name);
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
					*/
				}
			}else{
				if(aref->Columns && aref->Rows && (aref->X2 - aref->X1) && (aref->Y3 - aref->Y1)){
					dx = (float)(aref->X2 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y3 - aref->Y1) / (float)aref->Rows;

					if(obj){
						for(i=0; i<aref->Rows; i++){
							for(j=0; j<aref->Columns; j++){
								obj->OutputToOGL(optr, Objects, Font, offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, process);
							}
						}
					}
					/*
					fprintf(tr, "#declare dx = %.2f;\n", dx);
					fprintf(fptr, "#declare dy = %.2f;\n", dy);

					fprintf(fptr, "#declare colcount = 0;\n");
					fprintf(fptr, "#declare cols = %d;\n", aref->Columns);
					fprintf(fptr, "#declare rows = %d;\n", aref->Rows);
					fprintf(fptr, "#while (colcount < cols)\n");
					fprintf(fptr, "\t#declare rowcount = 0;");
					fprintf(fptr, "\t#while (rowcount < rows)\n");
					fprintf(fptr, "\t\tobject{str_%s ", aref->Name);
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
					*/
				}
			}
		}
	}
}

void GDSObject::OutputToOGL(FILE *optr, class GDSObjects *Objects, char *Font, float offx, float offy, class GDSProcess *process)
{
	OutputOGLVertices(optr, offx, offy, process);

	if(FirstSRef){
		OutputOGLSRefs(optr, Objects, Font, offx, offy, process);
	}
	if(FirstARef){
		OutputOGLARefs(optr, Objects, Font, offx, offy, process);
	}
}

