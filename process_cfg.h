#ifndef _PROCESS_CFG_H
#define _PROCESS_CFG_H

#include <string>

struct ProcessLayer{
	struct ProcessLayer *Next;
	char *Name;
	int Layer;
	int Datatype;
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

	int Valid;
public:
	GDSProcess (char *filename);
	~GDSProcess ();

	void AddLayer(struct ProcessLayer *NewLayer);
	struct ProcessLayer *GetLayer(int Number, int Datatype);
	int LayerCount();
	int IsValid();
	float GetHighest();
	float GetLowest();
};

#endif // _PROCESS_CFG_H
