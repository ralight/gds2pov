#ifndef _PROCESS_CFG_H
#define _PROCESS_CFG_H

#include <string>

#ifdef WIN32
#define __BYTE_ORDER 1
#define __LITTLE_ENDIAN 1
#endif

struct ProcessLayer{
	struct ProcessLayer *Next;
	char *Name;
	int Layer;
	float Height;
	float Thickness;
	int Show;
	float Red;
	float Green;
	float Blue;
	float Filter;
	int Metal;
};

typedef struct ProcessLayer layers;

class GDSProcess
{
private:
	struct ProcessLayer *FirstLayer;
	int Count;

public:
	GDSProcess (char *filename);
	~GDSProcess ();

	void AddLayer(struct ProcessLayer *NewLayer);
	struct ProcessLayer *GetLayer(int Number);
	int LayerCount();
};

#endif // _PROCESS_CFG_H
