#ifndef _CONFIG_CFG_H
#define _CONFIG_CFG_H

#include <string>

typedef enum {
	ptNone,
	ptCamera,
	ptLookAt,
	ptLight
} PosType;

typedef enum {
	bpCentre,
	bpTopLeft,
	bpTopRight,
	bpBottomLeft,
	bpBottomRight
} BoundaryPos;

typedef struct _Position{
	struct _Position *Next;
	PosType postype;
	BoundaryPos boundarypos;
	float XMod;
	float YMod;
	float ZMod;
} Position;

class GDSConfig
{
private:
	Position CameraPos;
	Position LookAtPos;
	Position *FirstLight;
	Position *LastLight;

	int LightCount;

	int Valid;

	char *Font;
	char *ProcessFile;
	float Ambient;
public:
	GDSConfig(char *filename);
	GDSConfig();
	~GDSConfig();

	char *GetProcessFile();
	float GetAmbient();
	int IsValid();
	char *GetFont();

	Position *GetLookAtPos();
	Position *GetCameraPos();
	Position *GetLightPos();
};

#endif // _PROCESS_CFG_H

