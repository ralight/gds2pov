#ifndef __GDSPARSE_H__
#define __GDSPARSE_H__

#include <stdarg.h>
#include <string>
using namespace std;

#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsobject.h"
#include "gdsobjects.h"
#include "gds_types.h"

#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>

struct mhtime
{
    unsigned char blob[16];
};
struct htime
{
    struct timeval start;
};

typedef enum{
	cpCentre,
	cpBottomLeft,
	cpBottomRight,
	cpTopLeft,
	cpTopRight
} CameraPosition;

class GDSParse
{
private:
	char *libname;
	char *topcellname;

	short currentlayer;
	float currentwidth;
	short currentpathtype;
	gds_element_type currentelement;
	short currenttexttype;
	short currentpresentation;
	char *textstring;
	unsigned short currentstrans;
	float currentangle;
	short currentdatatype;
	float currentmag;
	float currentbgnextn;
	float currentendextn;

	char *sname;
	short arrayrows, arraycols;
	float units;
	float angle;
	FILE *iptr;
	FILE *optr;
	class GDSProcess *process;
	class GDSConfig *config;
	
	short recordlen;

	bool unsupported[70];

	long PathElements;
	long BoundaryElements;
	long TextElements;
	long SRefElements;
	long ARefElements;

	class GDSObjects *Objects;
	class GDSObject *CurrentObject;

	/* gds_parse.h functions */
	void ParseHeader();
	void ParseLibName();
	void ParseSName();
	void ParseUnits();
	void ParseStrName();
	void ParseXY();
	void ParseXYPath();
	void ParseXYBoundary();
	void ParseSTrans();

	bool Parse(char *infile, char *topcell);
	void Parse();
	void HandleSRef();
	void HandleARef();
	void HandleBoundary();
	void HandlePath();

	short GetBitArray();
	double GetEightByteReal();
	long GetFourByteSignedInt();
	short GetTwoByteSignedInt();
	char *GetAsciiString();

	void ReportUnsupported(char *Name, enum RecordNumbers rn);
	
	/* POV */
	void OutputPOVHeader();
	/* End of POV */

	/* 3DS */
	void Output3DSHeader();
	void Output3DSFooter();
	void Output3DSCamera(struct _Boundary *Boundary);
	void Output3DSLights(struct _Boundary *Boundary);
	void Output3DSMaterials();
	/* End of 3DS */

	/* OpenGL */
	void OutputOGLHeader();
	void OutputOGLFooter();
	void gl_draw(class GDSObject *Object);
	int gl_main(class GDSObject *Object);
	void gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
                GLint x, GLint y, GLuint font, const char *format, ... );
	void gl_event( int event, int data, int xpos, int ypos );
	void gl_resize( void );
	int gl_init( void );
	int gl_data( void );
	int glx_init( int fullscreen );
	void hide_mouse( void );
	void move_mouse( int x, int y );
	float timer( struct htime *t, int reset );

int width, height;

int info;
int frames;
GLuint font;
GLfloat fps;
GLfloat rx, ry;
GLfloat x, y, z;
GLfloat vx, vy, vz;
struct htime tv, mt;

Display *dpy;
Window win;
Atom wmDelete;
GLXContext ctx;
Cursor null_cursor;

int modeswitch;
int active;
int run;
	/* End of OpenGL */

	void GDSParse::Output3DSCameraLightPos(struct _Boundary *Boundary, BoundaryPos bp, float XMod, float YMod, float ZMod);
	void RecursiveOutput(class GDSObject *Object, FILE *optr, float offx, float offy, long *objectid);
public:
	GDSParse (char *infile, char *outfile, char *configfile, char *processfile, char *topcell);
	~GDSParse ();
};

#endif // __GDSPARSE_H__

