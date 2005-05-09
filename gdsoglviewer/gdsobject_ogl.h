#ifndef __GDSOBJECT_OGL_H__
#define __GDSOBJECT_OGL_H__

#include "gdsobject.h"

class GDSObject_ogl : public GDSObject
{
public:
	GDSObject_ogl(char *Name);
	~GDSObject_ogl();

	virtual void OutputToFile(FILE *fptr, class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputOGLVertices(float offx, float offy);
	void OutputOGLSRefs(class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);
	void OutputOGLARefs(class GDSObjects *Objects, char *Font, float offx, float offy, long *objectid, struct ProcessLayer *firstlayer);

};

#endif // __GDSOBJECT_OGL_H__

