#ifndef __GDSELEMENTS_H__
#define __GDSELEMENTS_H__

typedef struct {
	float X;
	float Y;
	float Angle;
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

typedef struct _SRefElement {
	struct _SRefElement *Next;
	float X;
	float Y;
	float Mag;
	char *Name;
	Transform Rotate;
	int Flipped;
	class GDSObject *object;
} SRefElement;

typedef struct _ARefElement {
	struct _ARefElement *Next;
	float X1;
	float Y1;
	float X2;
	float Y2;
	float X3;
	float Y3;
	float Mag;
	int Columns;
	int Rows;
	char *Name;
	Transform Rotate;
	int Flipped;
	class GDSObject *object;
} ARefElement;

struct _Boundary {
	float XMin;
	float XMax;
	float YMin;
	float YMax;
};

#endif // __GDSELEMENTS_H__
