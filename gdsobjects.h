#ifndef __GDSOBJECTS_H__
#define __GDSOBJECTS_H__

#include <stdarg.h>
#include <string>
using namespace std;

#include "gdsobject.h"

struct ObjectList{
	struct ObjectList *Next;
	struct ObjectList *Prev;
	class GDSObject *Object;
};

class GDSObjects
{
private:
	struct ObjectList *FirstObject;
	struct ObjectList *LastObject;
	struct _Boundary *Boundary;
	int Count;

public:
	GDSObjects();
	~GDSObjects();

	class GDSObject *AddObject(char *Name);
	class GDSObject *GetObject(int Index);
	class GDSObject *GetObject(char *Name);
	struct _Boundary *GetBoundary();
	int GetCount();
};

#endif // __GDSOBJECTS_H__
