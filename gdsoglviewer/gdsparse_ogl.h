/*
 * File: gdsparse_ogl.h
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef GDSPARSE_OGL_H
#define GDSPARSE_OGL_H

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
#ifdef HAVE_GETTIMEOFDAY
	struct timeval start;
#else
#ifdef HAVE_WINDOWS_H
	LARGE_INTEGER start, hfreq;
#endif
#endif /* HAVE_GETTIMEOFDAY */
};

class GDSParse_ogl : public GDSParse
{
private:
	bool m_bounding_output;

	int m_info;
	int m_frames;
	GLfloat m_fps;
	GLfloat m_rx, m_ry;
	GLfloat m_x, m_y, m_z;
	GLfloat m_vx, m_vy, m_vz;
	GLfloat m_dir[8][3];
	GLfloat m_phi[8][3];

	GLfloat m_speed_factor; /* to have a similar speed independent of the feature size */
	GLfloat m_xmin,m_xmax,m_ymin,m_ymax; /* the bounding box for the gds features, FIXME: include z values */

	struct htime m_tv, m_mt;

#ifdef HAVE_X11_XLIB_H
	Display *dpy;
	Window win;
	Atom wmDelete;
	GLXContext ctx;
	Cursor null_cursor;
#endif
public:
	GLuint m_font;
	int m_width, m_height;

	int m_modeswitch;
	int m_active;
	int m_run;

#ifdef HAVE_WINDOWS_H
	HDC m_hDC;
	HGLRC m_hRC;
	HWND m_hWnd;
#endif


	GDSParse_ogl (class GDSConfig *config, class GDSProcess *process, bool generate_process);
	~GDSParse_ogl ();

	//class GDSObject_ogl *NewObject(std::string Name);
	class GDSObject *NewObject(std::string Name);
	void OutputHeader();
	void OutputFooter();

	void SetTopcell(std::string topcell);
	int gl_data();
	int gl_init();
	void gl_draw();
	void gl_printf(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha, \
                GLint x, GLint y, GLuint font, const char *format, ... );
	void gl_resize(void);
	void gl_event( int event, int data, int xpos, int ypos );
	int gl_main();
	int glx_init(int fullscreen);

	float timer(struct htime *t, int reset);
	void hide_mouse(void);
	void move_mouse(int x, int y);
};

#endif
