#ifndef __GDSOBJECT_H__
#define __GDSOBJECT_H__

#include <stdarg.h>
using namespace std;
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>

#include "process_cfg.h"
#include "gdselements.h"
#include "gdspath.h"
#include "gdstext.h"
#include "gdspolygon.h"

class GDSObject
{
private:
	vector<class GDSPath*> PathItems;
	vector<class GDSText*> TextItems;
	vector<class GDSPolygon*> PolygonItems;

	SRefElement *FirstSRef;
	SRefElement *LastSRef;
	ARefElement *FirstARef;
	ARefElement *LastARef;
	bool GotBoundary;
	bool IsOutput;
	int SRefCount, ARefCount;

	char *Name;

	struct _Boundary Boundary;

	class GDSObject **SRefs;
	class GDSObject **ARefs;
public:
	GDSObject(char *Name);
	~GDSObject();

	void AddText(float newX, float newY, float newZ, bool newFlipped, float newMag, int newVJust, int newHJust, struct ProcessLayer *newlayer);
	class GDSText *GetCurrentText();

	void AddPolygon(float Height, float Thickness, int Points, struct ProcessLayer *layer);
	class GDSPolygon *GetCurrentPolygon();

	void AddSRef(char *Name, float X, float Y, int Flipped, float Mag);
	void SetSRefRotation(float X, float Y, float Z);

	void AddARef(char *Name, float X1, float Y1, float X2, float Y2, float X3, float Y3, int Columns, int Rows, int Flipped, float Mag);
	void SetARefRotation(float X, float Y, float Z);

	void AddPath(int PathType, float Height, float Thickness, int Points, float Width, float BgnExtn, float EndExtn, struct ProcessLayer *layer);
	class GDSPath *GetCurrentPath();

	char *GetName();

	/* POV */
	void OutputToPOV(FILE *fptr, class GDSObjects *Objects, char *Font);
	void DecomposePOVPolygons(FILE *fptr);
	/* End of POV */

	/* 3DS */
	void OutputTo3DS(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void Output3DSVertices(FILE *fptr, float offx, float offy, unsigned long *facecount);
	void Output3DSFaces(FILE *fptr, unsigned long *facecount, struct ProcessLayer *firstlayer);
	void Output3DSFaceMaterials(FILE *fptr, unsigned long facecount, struct ProcessLayer *firstlayer);
	void Output3DSSRefs(FILE *fptr, GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void Output3DSARefs(FILE *fptr, GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	/* End of 3DS */

	/* OpenGL */
	void OutputToOGL(FILE *optr, class GDSObjects *Objects, char *Font, float offx, float offy, class GDSProcess *process);
	void OutputOGLVertices(FILE *optr, float offx, float offy, class GDSProcess *process);
	void OutputOGLFaceMaterials(unsigned long facecount, struct ProcessLayer *firstlayer);
	void OutputOGLSRefs(FILE *optr, class GDSObjects *Objects, char *Font, float offx, float offy, class GDSProcess *process);
	void OutputOGLARefs(FILE *optr, class GDSObjects *Objects, char *Font, float offx, float offy, class GDSProcess *process);
	/* End of OpenGL */

	int HasASRef();
	class GDSObject *GetSRef(class GDSObjects *Objects, int Index);
	class GDSObject *GetARef(class GDSObjects *Objects, int Index);
	void IndexSRefs(class GDSObjects *Objects);
	void IndexARefs(class GDSObjects *Objects);

	struct _Boundary *GetBoundary(struct ObjectList *objectlist);

	bool GetIsOutput();
};

#endif // __GDSOBJECT_H__

