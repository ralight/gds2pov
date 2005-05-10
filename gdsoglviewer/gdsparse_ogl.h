#ifndef __GDSPARSE_OGL_H__
#define __GDSPARSE_OGL_H__

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"

#ifdef HAVE_WINDOWS_H
struct htime{
	LARGE_INTEGER start, hfreq;
};
#endif

struct timer{
	struct timeval start;
};

class GDSParse_ogl : public GDSParse
{
private:
	bool _bounding_output;


	int _info;
	int _frames;
	GLfloat _fps;
	GLfloat _rx, _ry;
	GLfloat _x, _y, _z;
	GLfloat _vx, _vy, _vz;
	GLfloat _dir[8][3];
	GLfloat _phi[8][3];
	struct htime _tv, _mt;

public:
	GLuint _font;
	int _width, _height;
#ifdef HAVE_WINDOWS_H
	HDC _hDC;
	HGLRC _hRC;
	HWND _hWnd;

	int _modeswitch;
	int _active;
	int _run;
#endif


	GDSParse_ogl (class GDSConfig *config, class GDSProcess *process);
	~GDSParse_ogl ();

	//class GDSObject_ogl *NewObject(char *Name);
	class GDSObject *NewObject(char *Name);
	void OutputHeader();
	void OutputFooter();

	int gl_data();
	int gl_init();
	void gl_draw();
	void gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, \
                GLint x, GLint y, GLuint font, const char *format, ... );
	void gl_resize(void);
	void gl_event( int event, int data, int xpos, int ypos );
	int gl_main(class GDSObject *Object);
	int glx_init(int fullscreen);

#ifdef HAVE_WINDOWS_H
	float timer(struct htime *t, int reset);
#endif
	float timer(struct timer *t, int reset);
	void hide_mouse(void);
	void move_mouse(int x, int y);
};

#endif // __GDSPARSE_OGL_H__

