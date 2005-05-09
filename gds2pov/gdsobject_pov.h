#ifndef __GDSOBJECT_POV_H__
#define __GDSOBJECT_POV_H__

#include "gdsobject.h"

class GDSObject_pov : public GDSObject
{
public:
	GDSObject_pov(char *Name);
	~GDSObject_pov();

	void DecomposePOVPolygons(FILE *fptr);
	virtual void OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputPathToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputPolygonToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputTextToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputSRefToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputARefToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);

};

#endif // __GDSOBJECT_POV_H__

