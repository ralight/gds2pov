#ifndef _CONFIG_CFG_H
#define _CONFIG_CFG_H

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
	Position	_CameraPos;
	Position	_LookAtPos;
	Position	*_FirstLight;
	Position	*_LastLight;

	int		_LightCount;

	int		_Valid;

	char		*_Font;
	char		*_ProcessFile;
	float		_Ambient;

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

