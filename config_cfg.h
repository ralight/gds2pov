#ifndef _CONFIG_CFG_H
#define _CONFIG_CFG_H

#include <string>

typedef enum {
	ptCamera,
	ptLookAt,
	ptLight
} PosType;

typedef struct _Position{
	struct _Position *Next;
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
} Position;

class GDSConfig
{
private:
	Position CameraPos;
	Position LookAtPos;
	Position *LightPos;

	int LightCount;

	int Valid;

	char *ProcessFile;
	float Ambient;
public:
	GDSConfig(char *filename);
	GDSConfig();
	~GDSConfig();

	char *GetProcessFile();
	float GetAmbient();
};

#endif // _PROCESS_CFG_H
