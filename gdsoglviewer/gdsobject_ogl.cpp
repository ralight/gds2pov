/*
 * File: gdsobject_ogl.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the OpenGL viewer specific implementation of the GDSObject class.
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
#include <cstdlib>
#include <cmath>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_GL_GL_H
#  include <GL/gl.h>
#endif
#ifdef HAVE_GL_GLU_H
#  include <GL/glu.h>
#endif

#include "gds_globals.h"
#include "gdsoglviewer.h"
#include "gdsobject_ogl.h"
#include "gdsobjects.h"
#include "process_cfg.h"

typedef struct{
	float X;
	float Y;
	float Z;
} Point3D;

GDSObject_ogl::GDSObject_ogl(char *Name) : GDSObject(Name){
}

GDSObject_ogl::~GDSObject_ogl()
{
}

void GDSObject_ogl::OutputOGLVertices(float offx, float offy)
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

	if(!PathItems.empty()){
		glBegin(GL_TRIANGLES);
		for(unsigned long i=0; i<PathItems.size(); i++){
			path = PathItems[i];

			layer = path->GetLayer();
			int plist[] = {7,4,5, 5,6,7, 2,1,0, 0,3,2, 6,5,1, 1,2,6, 4,7,3, 3,0,4, 6,2,3, 3,7,6, 4,0,1, 1,5,4};
			if(path->GetWidth() && layer){
				glPolygonMode(GL_FRONT_AND_BACK, render_mode);
				glColor3f(layer->Red, layer->Green, layer->Blue);

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
				for(unsigned long j=0; j<path->GetPoints()-1; j++){
					dx = path->GetXCoords(j) - path->GetXCoords(j+1);
					dy = path->GetYCoords(j+1) - path->GetYCoords(j);

/* 
					if(dx < 0.001){
						angleX = (dy < 0.0)? -1.0:1.0;
						angleY = 0.0;
						printf("a dx %f, dy %f\n", dx, dy);
					}else if(dy < 0.001){
						angleX = 0.0;
						angleY = (dx < 0.0)? -1.0:1.0;
						printf("b dx %f, dy %f\n", dx, dy);
					}else if(dx + dy < 0.001){
						if(dx < 0.0){
							angleX = 0.707107f;
							angleY = -0.707107f;
						printf("c dx %f, dy %f\n", dx, dy);
						}else{
							angleX = -0.707107f;
							angleY = 0.707107f;
						printf("d dx %f, dy %f\n", dx, dy);
						}
					}else if(dx - dy < 0.001){
						if(dx < 0.0){
							angleX = -0.707107f;
							angleY = -0.707107f;
						printf("e dx %f, dy %f\n", dx, dy);
						}else{
							angleX = 0.707107f;
							angleY = 0.707107f;
						printf("f dx %f, dy %f\n", dx, dy);
						}
					}else{
 
						angleX = cos(atan2(dx, dy));
						angleY = sin(atan2(dx, dy));
						printf("g dx %f, dy %f\n", dx, dy);
					}
*/

					/* This is only run once each time the program is run, so we can more afford the expensive trig functions.
					   Sort the above out properly later. */
					angleX = cos(atan2(dx, dy));
					angleY = sin(atan2(dx, dy));


					if(j==0 || j==path->GetPoints()-2){
						extn_x = EndExtn * angleY;
					extn_y = EndExtn * angleX;
					}else{
						extn_x = 0.0;
						extn_y = 0.0;
					}

					// 1
					points[0].X = path->GetXCoords(j) + path->GetWidth() * angleX + extn_x;// + offx;
					points[0].Y = path->GetYCoords(j) + path->GetWidth() * angleY - extn_y;// + offy;
					points[0].Z = path->GetHeight() + path->GetThickness();
					// 5
					points[4].X = points[0].X;
					points[4].Y = points[0].Y;
					points[4].Z = path->GetHeight();
					// 2
					points[1].X = path->GetXCoords(j) - path->GetWidth() * angleX + extn_x;// + offx;
					points[1].Y = path->GetYCoords(j) - path->GetWidth() * angleY - extn_y;// + offy;
					points[1].Z = path->GetHeight() + path->GetThickness();
					// 6
					points[5].X = points[1].X;
					points[5].Y = points[1].Y;
					points[5].Z = path->GetHeight();
					// 3 
					points[2].X = path->GetXCoords(j+1) - path->GetWidth() * angleX - extn_x;// + offx;
					points[2].Y = path->GetYCoords(j+1) - path->GetWidth() * angleY - extn_y;// + offy;
					points[2].Z = path->GetHeight() + path->GetThickness();
					// 7
					points[6].X = points[2].X;
					points[6].Y = points[2].Y;
					points[6].Z = path->GetHeight();
					// 4
					points[3].X = path->GetXCoords(j+1) + path->GetWidth() * angleX - extn_x;// + offx;
					points[3].Y = path->GetYCoords(j+1) + path->GetWidth() * angleY - extn_y;// + offy;
					points[3].Z = path->GetHeight() + path->GetThickness();
					// 8
					points[7].X = points[3].X;
					points[7].Y = points[3].Y;
					points[7].Z = path->GetHeight();

					for(unsigned int k=0; k<12; k++){
						glEdgeFlag(GL_TRUE);
						glVertex3f(points[plist[0+3*k]].X,points[plist[0+3*k]].Y, points[plist[0+3*k]].Z);
						glVertex3f(points[plist[1+3*k]].X,points[plist[1+3*k]].Y, points[plist[1+3*k]].Z);
						glEdgeFlag(GL_FALSE);
						glVertex3f(points[plist[2+3*k]].X,points[plist[2+3*k]].Y, points[plist[2+3*k]].Z);
					}
				}
			}
		}
		glEnd();
	}

	if(!PolygonItems.empty()){
		class GDSPolygon *polygon;

		glBegin(GL_TRIANGLES);
		for(unsigned long i=0; i<PolygonItems.size(); i++){
			polygon = PolygonItems[i];
			layer = polygon->GetLayer();

			//glPolygonMode(GL_FRONT_AND_BACK, render_mode);
			glPolygonMode(GL_FRONT, render_mode);
			glColor3f(layer->Red, layer->Green, layer->Blue);

			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				tempf = polygon->GetXCoords(j);// + offx;
				tempf = polygon->GetYCoords(j);// + offy;
				tempf = polygon->GetHeight() + polygon->GetThickness();
			}

			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				
				tempf = polygon->GetXCoords(j);// + offx;
				tempf = polygon->GetYCoords(j);// + offy;
				tempf = polygon->GetHeight();
			}

			float z1 = polygon->GetHeight();
			float z2 = polygon->GetHeight() + polygon->GetThickness();
			float x1, y1;
			float x2, y2;

			// FIXME - is this correct? 
			// Surely it should be GetPoints()-2
			for(unsigned int j=0; j<polygon->GetPoints()-1; j++){
				x1 = polygon->GetXCoords(j);// + offx;
				y1 = polygon->GetYCoords(j);// + offy;

				x2 = polygon->GetXCoords(j+1);// + offx;
				y2 = polygon->GetYCoords(j+1);// + offy;

				glEdgeFlag(GL_TRUE);
				glVertex3f(x2, y2, z1);
				glVertex3f(x1, y1, z1);
				glEdgeFlag(GL_FALSE);
				glVertex3f(x1, y1, z2);

				glEdgeFlag(GL_TRUE);
				glVertex3f(x1, y1, z2);
				glVertex3f(x2, y2, z2);
				glEdgeFlag(GL_FALSE);
				glVertex3f(x2, y2, z1);
			}
			DecomposePolygons(polygon);
/*
			x1 = polygon->GetXCoords(polygon->GetPoints()-2);// + offx;
			y1 = polygon->GetYCoords(polygon->GetPoints()-2);// + offy;

			x2 = polygon->GetXCoords(0);// + offx;
			y2 = polygon->GetYCoords(0);// + offy;

			glEdgeFlag(GL_TRUE);
			glVertex3f(x2, y2, z1);
			glVertex3f(x1, y1, z1);
			glEdgeFlag(GL_FALSE);
			glVertex3f(x1, y1, z2);

			glEdgeFlag(GL_TRUE);
			glVertex3f(x1, y1, z2);
			glVertex3f(x2, y2, z2);
			glEdgeFlag(GL_FALSE);
			glVertex3f(x2, y2, z1);
*/
		}
		glEnd();
	}
}

void GDSObject_ogl::OutputOGLSRefs(class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	GDSObject *obj;

	if(FirstSRef){
		SRefElement dummysref;
		dummysref.Next = FirstSRef;

		SRefElement *sref = &dummysref;

		while(sref->Next){
			sref = sref->Next;

			obj = Objects->GetObjectRef(sref->Name);
			//obj = sref->object; //FIXME - shouldn't use GetObject() as it is inefficient, should have object references assigned to sref->object instead
			if(obj){
				glPushMatrix();
				if(sref->Mag!=1.0){
					glScalef(sref->Mag, sref->Mag, 1);
				}
				glTranslatef(offx+sref->X, offy+sref->Y, 0.0f);
				if(sref->Rotate.Y){
				//	fprintf(NULL, "Rotate_Around_Trans(<0,0,%.2f>,<%.2f,%.2f,0>)", -sref->Rotate.Y, sref->X, sref->Y);
					glRotatef(-sref->Rotate.Y, 0.0f, 0.0f, 1.0f);
				}
				if(sref->Flipped){
					glScalef(1.0f, -1.0f, 1.0f);
				}
				obj->OutputToFile(NULL, Objects, Font, offx+sref->X, offy+sref->Y, objectid, firstlayer);
				glPopMatrix();
			}
		}
	}
}

void GDSObject_ogl::OutputOGLARefs(class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
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
			obj = Objects->GetObjectRef(aref->Name);
			//obj = aref->object; //FIXME - see same line for OutputOGLSRefs()
			if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
				if(aref->Columns && aref->Rows && (aref->X3 - aref->X1) && (aref->Y2 - aref->Y1)){
					dx = (float)(aref->X3 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y2 - aref->Y1) / (float)aref->Rows;

					if(obj){
						for(i=0; i<aref->Rows; i++){
							for(j=0; j<aref->Columns; j++){
								glPushMatrix();
								if(aref->Mag!=1.0){
									glScalef(aref->Mag, aref->Mag, 1);
								}
								//glTranslatef(offx+aref->X1, offy+aref->Y1, 0.0f);
								glTranslatef(offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, 0.0f);
								if(aref->Rotate.Y){
									glRotatef(-aref->Rotate.Y, 0.0f, 0.0f, 1.0f);
								}
								if(aref->Flipped){
									glScalef(1.0f, -1.0f, 1.0f);
								}
								//glTranslatef(dx*(float)j, dy*(float)i, 0.0f);
								//obj->OutputToFile(NULL, Objects, Font, offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, objectid, firstlayer);
								//obj->OutputToFile(NULL, Objects, Font, dx*(float)j, dy*(float)i, objectid, firstlayer);
								obj->OutputToFile(NULL, Objects, Font, 0.0,0.0, objectid, firstlayer);
								glPopMatrix();
							}
						}
					}
				}
			}else{
				if(aref->Columns && aref->Rows && (aref->X2 - aref->X1) && (aref->Y3 - aref->Y1)){
					dx = (float)(aref->X2 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y3 - aref->Y1) / (float)aref->Rows;

					if(obj){
						for(i=0; i<aref->Rows; i++){
							for(j=0; j<aref->Columns; j++){
								glPushMatrix();
								if(aref->Mag!=1.0){
									glScalef(aref->Mag, aref->Mag, 1);
								}
								//glTranslatef(offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, 0.0f);
								glTranslatef(offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, 0.0f);
								if(aref->Rotate.Y){
									glRotatef(-aref->Rotate.Y, 0.0f, 0.0f, 1.0f);
								}
								if(aref->Flipped){
									glScalef(1.0f, -1.0f, 1.0f);
								}
								//glTranslatef(offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, 0.0f);
								obj->OutputToFile(NULL, Objects, Font, offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, objectid, firstlayer);
								glPopMatrix();
							}
						}
					}
				}
			}
		}
	}
}

void GDSObject_ogl::OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)
{
	OutputOGLVertices(offx, offy);

	if(FirstSRef){
		OutputOGLSRefs(Objects, Font, offx, offy, objectid, firstlayer);
	}
	if(FirstARef){
		OutputOGLARefs(Objects, Font, offx, offy, objectid, firstlayer);
	}
}

void GDSObject_ogl::DecomposePolygons(class GDSPolygon *polygon)
{
	unsigned long faceindex = 0;

	if(!polygon){
		return;
	}
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
		polygon->SetAngleCoords(j, (float)asin(sin(polygon->GetAngleCoords(j))));
		if(polygon->GetAngleCoords(j)>=0.0){
			positives++;
		}else{
			negatives++;
		}
	}
	
	//printf("+ve %d, -ve %d\n", positives, negatives);
	int bendindex1;
	int bendindex2;

	if(!positives || !negatives){
		float z1 = polygon->GetHeight();
		float z2 = polygon->GetHeight() + polygon->GetThickness();

		for(unsigned int j=1; j<polygon->GetPoints()-2; j++){
			glEdgeFlag(GL_TRUE);
			glVertex3f(polygon->GetXCoords(0),polygon->GetYCoords(0),z1);
			glVertex3f(polygon->GetXCoords(j),polygon->GetYCoords(j),z1);
			glVertex3f(polygon->GetXCoords(j+1),polygon->GetYCoords(j+1),z1);

			glVertex3f(polygon->GetXCoords(0),polygon->GetYCoords(0),z2);
			glVertex3f(polygon->GetXCoords(j),polygon->GetYCoords(j),z2);
			glVertex3f(polygon->GetXCoords(j+1),polygon->GetYCoords(j+1),z2);

			glVertex3f(polygon->GetXCoords(0),polygon->GetYCoords(0),z2);
			glVertex3f(polygon->GetXCoords(j),polygon->GetYCoords(j),z2);
			glVertex3f(polygon->GetXCoords(j+1),polygon->GetYCoords(j+1),z2);
			glEdgeFlag(GL_FALSE);
			//glVertex3f(,,);
			//0, j, j+1
			//polygon->GetPoints()-1, j+polygon->GetPoints()-1, j+polygon->GetPoints()-1+1
		}
	}
}

