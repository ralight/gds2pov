#ifndef __GDSOBJECT_H__
#define __GDSOBJECT_H__

#include <stdarg.h>
#include <string>
using namespace std;

#include "process_cfg.h"

typedef struct {
	float X;
	float Y;
} Point;

typedef struct {
	float X;
	float Y;
	float Z;
} Transform;

typedef struct {
	float R;
	float G;
	float B;
	float F;
	int Metal;
} ElementColour;

typedef struct _Prism {
	struct _Prism *Next;
	float Height;
	float Thickness;
	int Points;
	float Width;			/* For paths only */
	Point *Coords;
	Transform Rotate;
	ElementColour Colour;
} Prism;

typedef struct _TextElement {
	struct _TextElement *Next;
	float X;
	float Y;
	float Z;
	ElementColour Colour;
	char *String;
	Transform Rotate;
	int Flipped;
} TextElement;

typedef struct _SRefElement {
	struct _SRefElement *Next;
	float X;
	float Y;
	char *Name;
	Transform Rotate;
	int Flipped;
} SRefElement;

typedef struct _ARefElement {
	struct _ARefElement *Next;
	float X1;
	float Y1;
	float X2;
	float Y2;
	float X3;
	float Y3;
	int Columns;
	int Rows;
	char *Name;
	Transform Rotate;
	int Flipped;
} ARefElement;

struct _Boundary {
	float XMin;
	float XMax;
	float YMin;
	float YMax;
};

class GDSObject
{
private:
	TextElement *FirstText;
	TextElement *LastText;
	SRefElement *FirstSRef;
	SRefElement *LastSRef;
	ARefElement *FirstARef;
	ARefElement *LastARef;
	Prism *FirstPrism;
	Prism *LastPrism;
	Prism *FirstPath;
	Prism *LastPath;
	int GotBoundary;

	char *Name;

	struct _Boundary Boundary;

public:
	GDSObject(char *Name);
	~GDSObject();

	void AddPrism(float Height, float Thickness, int Points);
	void AddPrismPoint(int Index, float X, float Y);
	void SetPrismColour(float R, float G, float B, float F, int Metal);
	void SetPrismRotation(float X, float Y, float Z);

	void AddText(float X, float Y, float Z, int Flipped);
	void SetTextColour(float R, float G, float B, float F, int Metal);
	void SetTextString(char *String);
	void SetTextRotation(float X, float Y, float Z);

	void AddSRef(char *Name, float X, float Y, int Flipped);
	void SetSRefRotation(float X, float Y, float Z);

	void AddARef(char *Name, float X1, float Y1, float X2, float Y2, float X3, float Y3, int Columns, int Rows, int Flipped);
	void SetARefRotation(float X, float Y, float Z);

	void AddPath(float Height, float Thickness, int Points, float Width);
	void AddPathPoint(int Index, float X, float Y);
	void SetPathColour(float R, float G, float B, float F, int Metal);
	void SetPathRotation(float X, float Y, float Z);

	char *GetName();
	void OutputToFile(FILE *fptr);
	int HasASRef();

	struct _Boundary *GetBoundary(struct ObjectList *);
};

#endif // __GDSOBJECT_H__
