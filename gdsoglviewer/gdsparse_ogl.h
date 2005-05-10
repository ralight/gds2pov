#ifndef __GDSPARSE_OGL_H__
#define __GDSPARSE_OGL_H__

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_X11_XLIB_H
#  include <X11/Xlib.h>
#endif
#ifdef HAVE_GL_GLX_H
#  include <GL/glx.h>
#endif

#include "config_cfg.h"
#include "process_cfg.h"
#include "gdsparse.h"

struct htime{
#ifdef HAVE_WINDOWS_H
	LARGE_INTEGER start, hfreq;
#endif
#ifdef HAVE_GETTIMEOFDAY
	struct timeval start;
#endif
};

class GDSParse_ogl : public GDSParse
{
private:
	bool _bounding_output;
	bool _drawn; // have we created the OpenGL display list?


	int _info;
	int _frames;
	GLfloat _fps;
	GLfloat _rx, _ry;
	GLfloat _x, _y, _z;
	GLfloat _vx, _vy, _vz;
	GLfloat _dir[8][3];
	GLfloat _phi[8][3];
	struct htime _tv, _mt;

#ifdef HAVE_X11_XLIB_H
	Display *dpy;
	Window win;
	Atom wmDelete;
	GLXContext ctx;
	Cursor null_cursor;
#endif
public:
	GLuint _font;
	int _width, _height;

	int _modeswitch;
	int _active;
	int _run;

#ifdef HAVE_WINDOWS_H
	HDC _hDC;
	HGLRC _hRC;
	HWND _hWnd;
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

	float timer(struct htime *t, int reset);
	void hide_mouse(void);
	void move_mouse(int x, int y);
};

#endif // __GDSPARSE_OGL_H__

