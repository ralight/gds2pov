#ifndef __GDSOBJECT_H__
#define __GDSOBJECT_H__

#include <vector>
using namespace std;

#include "process_cfg.h"
#include "gdselements.h"
#include "gdspath.h"
#include "gdstext.h"
#include "gdspolygon.h"

class GDSObject
{
protected:
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
	virtual ~GDSObject();

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

	virtual void OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer)=0;
	//virtual void OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);

	int HasASRef();
	class GDSObject *GetSRef(class GDSObjects *Objects, int Index);
	class GDSObject *GetARef(class GDSObjects *Objects, int Index);
	void IndexSRefs(class GDSObjects *Objects);
	void IndexARefs(class GDSObjects *Objects);

	struct _Boundary *GetBoundary(struct ObjectList *objectlist);

	bool GetIsOutput();
};

#endif // __GDSOBJECT_H__

