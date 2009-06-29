#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gds_globals.h"
#include "gdsobject.h"
#include "gdsobjects.h"
#include "process_cfg.h"

void GDSObject::Output3DSVertices(FILE *fptr, float offx, float offy, unsigned long *facecount)
{
	unsigned short chunk;
	unsigned long chunklen;
	unsigned long chunklenpos;
	unsigned long vertexcount;
	unsigned long i, j;
	float angleX, angleY;
	float tempf;
	float BgnExtn;
	float EndExtn;
	float extn_x;
	float extn_y;

	if(!PathItems.empty() || !PolygonItems.empty()){
		chunk = 0x4110; // POINT_ARRAY
		chunklen = 0;
		fwrite(&chunk, 2, 1, fptr);
		chunklenpos = ftell(fptr);
		fwrite(&chunklen, 4, 1, fptr);
		vertexcount = 0;
		fwrite(&vertexcount, 2, 1, fptr);

		(*facecount) = 0;

		if(!PathItems.empty() && 0){
			class GDSPath *path;

			for(i=0; i<PathItems.size(); i++){
				path = PathItems[i];

				if(path->GetWidth()){
					vertexcount += 8*(path->GetPoints() - 1);
					(*facecount) += 12*(path->GetPoints() - 1);

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

					for(j=0; j<path->GetPoints()-1; j++){
						angleX = cos(atan2(path->GetXCoords(j) - path->GetXCoords(j+1), path->GetYCoords(j+1) - path->GetYCoords(j)));
						angleY = sin(atan2(path->GetXCoords(j) - path->GetXCoords(j+1), path->GetYCoords(j+1) - path->GetYCoords(j)));

						if(j==0 || j==path->GetPoints()-2){
							extn_x = EndExtn * angleY;
							extn_y = EndExtn * angleX;
						}else{
							extn_x = 0.0;
							extn_y = 0.0;
						}

						// 1
						tempf = path->GetXCoords(j) + path->GetWidth() * angleX + extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j) + path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight() - path->GetThickness();
						fwrite(&tempf, 4, 1, fptr);
						// 2
						tempf = path->GetXCoords(j) - path->GetWidth() * angleX + extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j) - path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight() - path->GetThickness();
						fwrite(&tempf, 4, 1, fptr);
						// 3
						tempf = path->GetXCoords(j) + path->GetWidth() * angleX + extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j) + path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight();
						fwrite(&tempf, 4, 1, fptr);
						// 4
						tempf = path->GetXCoords(j) - path->GetWidth() * angleX + extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j) - path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight();
						fwrite(&tempf, 4, 1, fptr);
	
						// 5
						tempf = path->GetXCoords(j+1) + path->GetWidth() * angleX - extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j+1) + path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight() - path->GetThickness();
						fwrite(&tempf, 4, 1, fptr);
						// 6
						tempf = path->GetXCoords(j+1) - path->GetWidth() * angleX - extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j+1) - path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight() - path->GetThickness();
						fwrite(&tempf, 4, 1, fptr);
						// 7
						tempf = path->GetXCoords(j+1) + path->GetWidth() * angleX - extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j+1) + path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight();
						fwrite(&tempf, 4, 1, fptr);
						// 8
						tempf = path->GetXCoords(j+1) - path->GetWidth() * angleX - extn_x + offx;
						fwrite(&tempf, 4, 1, fptr);
						tempf = path->GetYCoords(j+1) - path->GetWidth() * angleY - extn_y + offy;
						fwrite(&tempf, 4, 1, fptr);
						tempf = -path->GetHeight();
						fwrite(&tempf, 4, 1, fptr);
					}
				}
			}
		}

		if(!PolygonItems.empty()){
			class GDSPolygon *polygon;

			for(i=0; i<PolygonItems.size(); i++){
				polygon = PolygonItems[i];

				for(j=0; j<polygon->GetPoints()-1; j++){
					tempf = polygon->GetXCoords(j) + offx;
					fwrite(&tempf, 4, 1, fptr);
					tempf = polygon->GetYCoords(j) + offy;
					fwrite(&tempf, 4, 1, fptr);
					tempf = polygon->GetHeight() + polygon->GetThickness();
					fwrite(&tempf, 4, 1, fptr);

					vertexcount++;
				}

				for(j=0; j<polygon->GetPoints()-1; j++){
					
					tempf = polygon->GetXCoords(j) + offx;
					fwrite(&tempf, 4, 1, fptr);
					tempf = polygon->GetYCoords(j) + offy;
					fwrite(&tempf, 4, 1, fptr);
					tempf = polygon->GetHeight();
					fwrite(&tempf, 4, 1, fptr);

					vertexcount++;
				}
			}
		}

		//chunklen = 8 + vertexcount*12;
		chunklen = 8 + vertexcount*12;
		fseek(fptr, chunklenpos, SEEK_SET);
		fwrite(&chunklen, 4, 1, fptr);
		fwrite(&vertexcount, 2, 1, fptr);
		fseek(fptr, 0L, SEEK_END);
	}
}

void GDSObject::Output3DSFaces(FILE *fptr, unsigned long *facecount, class ProcessLayer *firstlayer)
{
	unsigned short chunk;
	unsigned long chunklen;
	unsigned long chunklenpos;
	unsigned long i, j;
	unsigned short tempshort;
	unsigned long faceindex;

	//if(!PathItems.empty() || !PolygonItems.empty()){
	if(!PolygonItems.empty()){
		chunk = 0x4120; // FACE_LIST
		chunklen = 0;
		fwrite(&chunk, 2, 1, fptr);
		chunklenpos = ftell(fptr);
		fwrite(&chunklen, 4, 1, fptr);
		// facecount for *paths* only calculated along with vertexcount in Output3DSVertices()
		fwrite(facecount, 2, 1, fptr);

		if(!PathItems.empty() && 0){
			int vertexindex[36] = {0, 1, 2, 1, 2, 3, 4, 5, 6, 5, 6, 7, 0, 1, 5, 0, 4, 5, 2, 3, 6, 3, 6, 7, 1, 3, 7, 1, 5, 7, 0, 2, 4, 2, 4, 6};

			class GDSPath *path;

			faceindex = 0;

			for(i=0; i<PathItems.size(); i++){
				path = PathItems[i];

				if(path->GetWidth()){
					for(i=0; i<path->GetPoints()-1; i++){
						for(j=0; j<12; j++){
							// print ,faces now
							tempshort = vertexindex[j*3] + 8*i + faceindex*8;
							fwrite(&tempshort, 2, 1, fptr);
							tempshort = vertexindex[j*3 + 1] + 8*i + faceindex*8;
							fwrite(&tempshort, 2, 1, fptr);
							tempshort = vertexindex[j*3 + 2] + 8*i + faceindex*8;
							fwrite(&tempshort, 2, 1, fptr);
							tempshort=7;
							fwrite(&tempshort, 2, 1, fptr);
						}
					}
					faceindex+=path->GetPoints()-1;
				}
			}
		}

		if(!PolygonItems.empty()){
			class GDSPolygon *polygon;

			faceindex = 0;

			for(i=0; i<PolygonItems.size(); i++){
				polygon = PolygonItems[i];

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

				for(j=1; j<polygon->GetPoints()-1; j++){
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
				for(j=0; j<polygon->GetPoints()-1; i++){
					polygon->SetAngleCoords(j, asinf(sinf(polygon->GetAngleCoords(j))));
					if(polygon->GetAngleCoords(j)>=0){
						positives++;
					}else{
						negatives++;
					}
				}
				if(!positives || !negatives){
					for(j=1; j<polygon->GetPoints()-2; j++){
						tempshort = 0;
						fwrite(&tempshort, 2, 1, fptr);
						tempshort = j;
						fwrite(&tempshort, 2, 1, fptr);
						tempshort = j+1;
						fwrite(&tempshort, 2, 1, fptr);
						tempshort=7;
						fwrite(&tempshort, 2, 1, fptr);
						
						(*facecount)++;
					}
				}

				for(j=0; j<polygon->GetPoints()-1; j++){
					tempshort = (unsigned short)i;
					fwrite(&tempshort, 2, 1, fptr);
					tempshort = (unsigned short)(j + polygon->GetPoints()-1);
					fwrite(&tempshort, 2, 1, fptr);
					tempshort = (unsigned short)((j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j:j+polygon->GetPoints());
					fwrite(&tempshort, 2, 1, fptr);

					tempshort = (unsigned short)j;
					fwrite(&tempshort, 2, 1, fptr);
					tempshort = (unsigned short)(j + 1);
					fwrite(&tempshort, 2, 1, fptr);
					tempshort = (unsigned short)((j+polygon->GetPoints()>=2*(polygon->GetPoints()-1))?j:j+polygon->GetPoints());
					fwrite(&tempshort, 2, 1, fptr);
				}

				// DEBUGGING printf("+ve %d, -ve %d\n\n", positives, negatives);
			}

		}

	//	Output3DSFaceMaterials(fptr, (*facecount), firstlayer);

		chunklen = 2 + ftell(fptr) - chunklenpos;
		fseek(fptr, chunklenpos, SEEK_SET);
		fwrite(&chunklen, 4, 1, fptr);
		fwrite(facecount, 2, 1, fptr);
		fseek(fptr, 0L, SEEK_END);
		//MATERIAL_LIST
		//MESH_MATRIX
	}
}

void GDSObject::Output3DSFaceMaterials(FILE *fptr, unsigned long facecount, class ProcessLayer *firstlayer)
{
	unsigned short chunk;
	unsigned long chunklen;
	unsigned long matlenpos;
	unsigned short tempshort;
	unsigned long i;
	unsigned char tempbyte;

	class ProcessLayer dummylayer;
	class ProcessLayer *thislayer;
	dummylayer.Next = firstlayer;
	thislayer = &dummylayer;
	
	unsigned short count;

	class GDSPath *path;

	while(thislayer->Next){
		thislayer = thislayer->Next;

		chunk = 0x4130; // add materials to faces
		chunklen = 0;
		fwrite(&chunk, 2, 1, fptr);
		matlenpos = ftell(fptr);
		fwrite(&chunklen, 4, 1, fptr);
		fprintf(fptr, "%s", thislayer->Name);
		tempbyte = 0;
		fwrite(&tempbyte, 1, 1, fptr);
		count = 0;
		fwrite(&count, 2, 1, fptr);

		facecount = 0;

		for(i=0; i<PathItems.size(); i++){
			path = PathItems[i];

			if(path->GetWidth()){
				if(thislayer == path->GetLayer()){
					for(i=0; i<12*(path->GetPoints()-1); i++){
						tempshort = i + facecount;
						fwrite(&tempshort, 2, 1, fptr);
					}
					count += 12*(path->GetPoints() - 1);
				}
				facecount += 12*(path->GetPoints() - 1);
			}
		}

		chunklen = 2 + ftell(fptr) - matlenpos;
		fseek(fptr, matlenpos, SEEK_SET);
		fwrite(&chunklen, 4, 1, fptr);
		fprintf(fptr, "%s", thislayer->Name);
		tempbyte = 0;
		fwrite(&tempbyte, 1, 1, fptr);
		fwrite(&count, 2, 1, fptr);
		fseek(fptr, 0L, SEEK_END);
	}
}

void GDSObject::Output3DSSRefs(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	GDSObject *obj;

	if(FirstSRef){
		SRefElement dummysref;
		dummysref.Next = FirstSRef;

		SRefElement *sref = &dummysref;

		while(sref->Next){
			sref = sref->Next;

			obj = Objects->GetObject(sref->Name);
			if(obj){
				obj->OutputTo3DS(fptr, Objects, Font, offx+sref->X, offy+sref->Y, objectid, firstlayer);
			}
	//		fprintf(fptr, "object{str_%s ", sref->Name);
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

void GDSObject::Output3DSARefs(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
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
			obj = Objects->GetObject(aref->Name);
			if(aref->Rotate.Y == 90.0 || aref->Rotate.Y == -90.0){
				if(aref->Columns && aref->Rows && (aref->X3 - aref->X1) && (aref->Y2 - aref->Y1)){
					dx = (float)(aref->X3 - aref->X1) / (float)aref->Columns;
					dy = (float)(aref->Y2 - aref->Y1) / (float)aref->Rows;

					if(obj){
						for(i=0; i<aref->Rows; i++){
							for(j=0; j<aref->Columns; j++){
								obj->OutputTo3DS(fptr, Objects, Font, offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, objectid, firstlayer);
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
								obj->OutputTo3DS(fptr, Objects, Font, offx+aref->X1+dx*(float)j, offy+aref->Y1+dy*(float)i, objectid, firstlayer);
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

void GDSObject::OutputTo3DS(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, class ProcessLayer *firstlayer)
{
	unsigned short chunk;
	unsigned long chunklen;
	unsigned char tempbyte;
	unsigned long objlenpos;
	unsigned long trilenpos;
	unsigned long facecount;

	if(fptr){
		objectid++;

		chunk = 0x4000;
		chunklen = 0;
		fwrite(&chunk, 2, 1, fptr);
		objlenpos = ftell(fptr);
		fwrite(&chunklen, 4, 1, fptr);
		fprintf(fptr, "str_%s_%ld", Name, (*objectid));
		tempbyte = 0;
		fwrite(&tempbyte, 1, 1, fptr);

		chunk = 0x4100; // N_TRI_OBJECT
		chunklen = 0;
		fwrite(&chunk, 2, 1, fptr);
		trilenpos = ftell(fptr);
		fwrite(&chunklen, 4, 1, fptr);

		Output3DSVertices(fptr, offx, offy, &facecount);
		Output3DSFaces(fptr, &facecount, firstlayer);

		chunklen = 2 + ftell(fptr) - objlenpos;
		fseek(fptr, objlenpos, SEEK_SET);
		fwrite(&chunklen, 4, 1, fptr);
		fseek(fptr, 0L, SEEK_END);
		
		chunklen = 2 + ftell(fptr) - trilenpos;
		fseek(fptr, trilenpos, SEEK_SET);
		fwrite(&chunklen, 4, 1, fptr);
		fseek(fptr, 0L, SEEK_END);

		if(FirstSRef){
			Output3DSSRefs(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		}
		if(FirstARef){
			Output3DSARefs(fptr, Objects, Font, offx, offy, objectid, firstlayer);
		}
	}
	IsOutput = true;
}

