#ifndef __GDSOBJECT_H__
#define __GDSOBJECT_H__

#include <stdarg.h>
#include <string>
using namespace std;

#include "process_cfg.h"

struct Element {
	struct Element *Next;
	char *line;
}

class GDSObject
{
private:
	struct Element *FirstElement;

public:
	GDSObject (char *filename);
	~GDSObject ();

	AddElement(char *line, int size);
	OutputToFile(FILE *fptr);
};

#endif // __GDSOBJECT_H__
