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
	int Count;				/* Number of layers found */

	int Valid;				/* Is the process file valid? */
public:
	GDSProcess (char *filename);
	~GDSProcess ();

	void 			Parse(char *processfile);
	//bool			Parse(char *processfile);

	void			AddLayer(struct ProcessLayer *NewLayer);
	struct ProcessLayer	*GetLayer(int Number, int Datatype);
	struct ProcessLayer	*GetLayer();
	struct ProcessLayer	*GetLayer(const char *Name);
	int			LayerCount();
	int			IsValid();
	float			GetHighest();
	float			GetLowest();
};

#endif // _PROCESS_CFG_H

