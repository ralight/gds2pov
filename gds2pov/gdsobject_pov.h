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

};

#endif // __GDSOBJECT_POV_H__

