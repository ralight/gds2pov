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
	int Height;
	int Thickness;
	int Show;
	int Metal;
	int Transparent;
	char *Colour;
};

class GDSProcess
{
private:
	struct ProcessLayer *FirstLayer;
	int Count;

public:
	GDSProcess (char *filename);
	~GDSProcess ();

	AddLayer(struct ProcessLayer *NewLayer);
	struct ProcessLayer *GetLayer(int Number);
	struct ProcessLayer *GetLayer(char *Name);
	int LayerCount();
};

#endif // _PROCESS_CFG_H
