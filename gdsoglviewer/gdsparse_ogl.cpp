/*
 * File: gdsparse_ogl.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the OpenGL viewer specific implementation of the GDSParse class.
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

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#ifdef HAVE_X11_KEYSYM_H
#include <X11/keysym.h>
#endif
#ifdef HAVE_X11_XLIB_H
#include <X11/Xlib.h>
#endif
#ifdef HAVE_GL_GLX_H
#include <GL/glx.h>
#endif
#ifdef HAVE_GL_GLU_H
#include <GL/glu.h>
#endif
#ifdef HAVE_GL_GL_H
#include <GL/gl.h>
#endif

#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#include <time.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <windows.h>
#endif

#include "gdsparse_ogl.h"
#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsoglviewer.h"
#include "gdsobject_ogl.h"

extern int verbose_output;

GDSParse_ogl::GDSParse_ogl(class GDSConfig *config, class GDSProcess *process, bool generate_process) :
		GDSParse(config, process, generate_process)
{
	m_bounding_output = false;
	m_use_outfile = false;
	m_allow_multiple_output = true;
	m_output_children_first = false;
}

GDSParse_ogl::~GDSParse_ogl()
{
}

class GDSObject *GDSParse_ogl::NewObject(std::string name)
{
	return new class GDSObject_ogl(name);
}

void GDSParse_ogl::SetTopcell(std::string topcell)
{
	m_topcellname = topcell;
}

void GDSParse_ogl::OutputHeader()
{
}

void GDSParse_ogl::OutputFooter()
{
}

/* application window title */

const char *AppTitle = "GDSto3D";

/* external OS-dependant functions */

/* global data */

/* data initialization function */

int GDSParse_ogl::gl_data()
{
	m_info = 1;

	m_x = m_rx = m_vx = 0.0f;
	m_y = m_ry = m_vy = 0.0f;
	m_z =	m_vz = 0.0f;

	m_speed_factor = 1;
	m_xmin = m_ymin = 0;
	m_xmax = m_ymax = 1;

	return(0);
}

/* gl initialization function */

int GDSParse_ogl::gl_init()
{
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);
	glLineWidth(2.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT, GL_FASTEST);
	//glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glPolygonMode(GL_FRONT_AND_BACK, render_mode);

	glNewList(1, GL_COMPILE);
	//glBegin(GL_TRIANGLES);
	Output(NULL, m_topcellname);
	//glEnd();
	glEndList();

	hide_mouse();
	move_mouse(m_width / 2, m_height / 2);
	timer(&m_mt, 1);


	/* init view position, same as in gds2pov */
	if(!m_objects.empty()){
		struct _Boundary *boundary = GetBoundary();

		float half_widthX = (boundary->xmax - boundary->xmin)/2;
		float half_widthY = (boundary->ymax - boundary->ymin)/2;
		float centreX = half_widthX + boundary->xmin;
		float centreY = half_widthY + boundary->ymin;

		float distance;
		if(half_widthX > half_widthY){
			distance = half_widthX * 1.8;
		}else{
			distance = half_widthY * 1.8;
		}

		m_speed_factor = distance/20.0f;
		m_xmin = boundary->xmin;
		m_xmax = boundary->xmax;
		m_ymin = boundary->ymin;
		m_ymax = boundary->ymax;

		float XMod = m_config->GetCameraPos()->xmod;
		float YMod = m_config->GetCameraPos()->ymod;
		float ZMod = m_config->GetCameraPos()->zmod;

		switch(m_config->GetCameraPos()->boundarypos){
			case bpCentre:
				// Default camera angle = 67.38
				// Half of this is 33.69
				// tan(33.69) = 0.66666 = 1/1.5
				// Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8

				m_x = centreX * XMod;
				m_y = centreY * YMod;
				m_z = distance * ZMod;
				break;
			case bpTopLeft:
				m_x = boundary->xmin * XMod;
				m_y = boundary->ymax * YMod;
				m_z = distance * ZMod;
				break;
			case bpTopRight:
				m_x = boundary->xmax*XMod;
				m_y = boundary->ymax*YMod;
				m_z = distance*ZMod;
				break;
			case bpBottomLeft:
				m_x = boundary->xmin * XMod;
				m_y = boundary->ymin * YMod;
				m_z = distance * ZMod;
				break;
			case bpBottomRight:
				m_x = boundary->xmax*XMod;
				m_y = boundary->ymin*YMod;
				m_z = distance*ZMod;
				break;
			}
		} /* end init view position */

	return(0);
}

/* window drawing function */
void GDSParse_ogl::gl_draw()
{
	GLfloat M[16], G[16];
	GLfloat R1x, R1y, R1z;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(m_fps){
	/*change angle according to velocity */
		m_ry += m_vy / m_fps;
		m_rx += m_vx / m_fps;

		/* slow velocity down ??*/
		if(m_fps > 4.0f){
			m_vy *= 1.0f - 4.0f / m_fps;
			m_vx *= 1.0f - 4.0f / m_fps;
		}else{
			m_vy = 0.0f;
			m_vx = 0.0f;
		}
	}

	/* make sure angles are in [-180, +180] */
	if(m_ry >180.0f) m_ry += -360.0f;
	if(m_ry < -180.0f) m_ry +=360.0f;

	if(m_rx >180.0f) m_rx += -360.0f;
	if(m_rx < -180.0f) m_rx +=360.0f;


	//FIXME glRotatef(_ry, 0.0f, 1.0f, 0.0f);
	glRotatef(m_ry, 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, M);

	/* save vector so we can calculate the inverse rotation later */
	R1x=M[0];R1y=M[4];R1z=M[8];
	glRotatef(m_rx, M[0], M[4], M[8]);
	glGetFloatv(GL_MODELVIEW_MATRIX, M);
	glFlush();

	if(m_fps){
		m_x -= M[2]* m_vz / m_fps;
		m_y -= M[6]* m_vz / m_fps;
		m_z -= M[10] * m_vz / m_fps;
	}

	/* FIXME: something is wrong, without the next chunk of code, theprogram starts up much faster, once it's up
	 it doesn't make a difference */
	{
	/* add an estimate for zFar, so that the whole obeject is always in view */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		{
			GLfloat tmpz;
			GLfloat x, y, z;

			/* distance to the bounding box, use larger value...
			if we would know the direction were are looking in, we could use the correct distance, but this works */
			x = (m_x-m_xmin);
			if ((m_xmax-m_x) > x) x = m_xmax-m_x;

			y = (m_y-m_ymin);
			if ((m_ymax-m_y) > y) y = m_ymax-m_y;

			/* FIXME: should use zmax and zmin instead of a default value of 50 */
			z = (m_z+50);
			if ((50-m_z) > z) z = 50-m_z;

			tmpz = x+y+z; /* correct would be \sqrt(x*x+y*y+z*z)+\epsilon, but this should be faster and as good */
			if(tmpz<10.0) tmpz = 10.0; /* not sure if neccessary */

			gluPerspective(50.0f , 1.0f, 1.0f, tmpz);/* set zFar */
		}
		glMatrixMode(GL_MODELVIEW); /* back to the model view*/
	}


	if(m_vz < -40.0f*m_speed_factor) m_vz = -40.0f*m_speed_factor;
	if(m_vz >40.0f*m_speed_factor) m_vz =40.0f*m_speed_factor;

/*
	if(_x < -6.0f) _x += 6.0f;
	if(_x >6.0f) _x -= 6.0f;
	if(_z < -6.0f) _z += 6.0f;
	if(_z >6.0f) _z -= 6.0f;
	if(_y < -0.7f) _y = -0.7f;
	if(_y >0.7f) _y =0.7f;
*/
	glTranslatef(-m_x, -m_y, -m_z);
	glGetFloatv(GL_MODELVIEW_MATRIX, M);

	glCallList(1);

	/* figure out look at statement: calculate inverse transformation and apply to (0, 0, -1) */
	glLoadIdentity();
	glTranslatef(m_x, m_y, m_z);
	glRotatef(-(m_rx), R1x, R1y, R1z);
	glRotatef(-(m_ry), 0.0f, 0.0f, 1.0f);
	glGetFloatv(GL_MODELVIEW_MATRIX, G);

	if(m_fps && m_info)
	{
		gl_printf(0.1f, 1.0f, 0.1f, 0.4f, m_width - 114, m_height - 40,
				 m_font, "fps: %5.1f", m_fps);
 		gl_printf(0.1f, 1.0f, 0.1f, 0.4f, m_width - 420, m_height - 40,
 				 m_font, "location:" );
 		gl_printf(0.1f, 1.0f, 0.1f, 0.4f, m_width - 320, m_height - 40,
			 m_font, " %5.1f %5.1f %5.1f", m_x, m_y, -(m_z) ); /*-z because of povrays coordinate system? */
 		gl_printf(0.1f, 1.0f, 0.1f, 0.4f, m_width - 420, m_height - 60,
 				 m_font, "look_at:" );
 		gl_printf(0.1f, 1.0f, 0.1f, 0.4f, m_width - 320, m_height - 60,
 				 m_font, " %5.1f %5.1f %5.1f", -G[8]+G[12], -G[9]+G[13], +G[10]-G[14] );

		if(m_fps < 20.0f){
			glDisable(GL_LINE_SMOOTH);
			glDisable(GL_POINT_SMOOTH);
			glDisable(GL_POLYGON_SMOOTH);
		}
	}

	glFinish();

	m_frames++;

	if(timer( &m_tv, 0) >= 0.2f)
	{
		m_fps = (GLfloat) m_frames / timer(&m_tv, 1);
		m_frames = 0;
	}
}

void GDSParse_ogl::gl_printf(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
				GLint x, GLint y, GLuint font, const char *format, ...)
{
	va_list argp;
	char text[256];

	va_start(argp, format);
	vsprintf(text, format, argp);
	va_end(argp);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();

		glLoadIdentity();
		gluOrtho2D(0.0, (GLdouble) m_width,
					0.0, (GLdouble) m_height);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glColor4f(red, green, blue, alpha);
		glRasterPos2i(x, y);
		glListBase(font);
		glCallLists(strlen( text), GL_UNSIGNED_BYTE, text);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
/* window resizing function */

void GDSParse_ogl::gl_resize(void)
{
	glViewport(0, 0, m_width, m_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLdouble) m_width / m_height, 0.1, 100.0);

	m_fps = 0.0f;
	m_frames = 0;
	timer(&m_tv, 1);
}

/* event handling function */

void GDSParse_ogl::gl_event(int event, int data, int xpos, int ypos)
{
	if(event == 0)	/* mouse button down */
	{
		if(data == 0) /* left button */
		{
			m_vz +=5.0f*m_speed_factor;
		}

		if(data == 1) /* right button */
		{
			m_vz += -5.0f*m_speed_factor;
		}
	}

	if(event == 1)	/* mouse button up */
	{
		m_vz = 0.0f;
	}

	if(event == 2)	/* mouse move */
	{
		m_vy +=128.0f * m_speed_factor * (GLfloat) (xpos - m_width/ 2) / m_width;
		m_vx -=128.0f * m_speed_factor * (GLfloat) (ypos - m_height / 2) / m_height;

		if(timer( &m_mt, 0) > 0.05)
		{
			timer(&m_mt, 1);
			move_mouse(m_width / 2, m_height / 2);
		}
	}

	if(event == 3)	/* key down */
	{
	}

	if(event == 4)	/* key up */
	{
		if(data == ' ')
		{
			m_info ^= 1;
		}
	}
}

/* external gl data */

/* variables associated with the window */


/* declaration of the GLX initialization function */

/* program entry point */

int GDSParse_ogl::gl_main()
{
#ifdef HAVE_GL_GLX_H
	int fullscreen;
	XEvent event;

	if(gl_data()){
		return(1);
	}

	fullscreen = 0;

	do{
		m_modeswitch= 0;
		fullscreen ^= 1;

		if(glx_init( fullscreen)){
			fprintf(stderr, "glx_init failed\n");
			return(1);
		}

		if(gl_init()){
			fprintf(stderr, "gl_init failed\n");
			return(1);
		}

		gl_resize();

		m_run = 1;

		while(m_run){
			if(m_active){
				gl_draw();
				glXSwapBuffers(dpy, win);
			}else{
				XPeekEvent(dpy, &event);
			}

			while(XPending( dpy)){
				XNextEvent(dpy, &event);

				switch(event.type){
					case ButtonPress:
					{
						int x = event.xmotion.x,
							y = event.xmotion.y;

						switch(event.xbutton.button)
						{
							case Button1: gl_event(0, 0, x, y); break;
							case Button3: gl_event(0, 1, x, y); break;
						}

						break;
					}

					case ButtonRelease:
					{
						int x = event.xmotion.x,
							y = event.xmotion.y;

						switch(event.xbutton.button)
						{
							case Button1: gl_event(1, 0, x, y); break;
							case Button3: gl_event(1, 1, x, y); break;
						}

						break;
					}

					case MotionNotify:
			{
						int x = event.xmotion.x,
							y = event.xmotion.y;

						switch(event.xbutton.button)
						{
							case Button1: gl_event(2,0, x, y); break;
							case Button3: gl_event(2,1, x, y); break;
							default:	gl_event(2, -1, x, y); break;
						}

						break;
					}

					case KeyPress:
					{
						break;
					}

					case KeyRelease:
					{
						int key = XLookupKeysym(&event.xkey, 0);

						switch(key)
						{
							case XK_Tab:
								m_modeswitch = 1;

							case XK_Escape:
								m_run = 0;
								break;

							default:

								gl_event(4, key, -1, -1);
								break;
						}

						break;
					}

					case UnmapNotify: m_active = 0; break;
					case MapNotify: m_active = 1; break;

					case ConfigureNotify:
					{
						m_width = event.xconfigure.width;
						m_height = event.xconfigure.height;
						gl_resize();
						break;
					}

					case ClientMessage:
					{
						if(event.xclient.data.l[0] == (int) wmDelete)
						{
							m_active = m_run = 0;
						}
						break;
					}

					case ReparentNotify: break;

					default:
					{
						printf("caught unknown event, type %d\n",
								event.type);
						break;
					}
				}
			}
		}

		glXMakeCurrent(dpy, None, NULL);
		glXDestroyContext(dpy, ctx);
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
	}
	while(m_modeswitch);
#endif // HAVE_GL_GLX_H

	return(0);
}

int GDSParse_ogl::glx_init(int fullscreen)
{
#ifdef HAVE_GL_GLX_H
	int vi_attr[] =
		{
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_RED_SIZE, 	 4,
			GLX_GREEN_SIZE, 	 4,
			GLX_BLUE_SIZE, 	4,
			GLX_DEPTH_SIZE, 	16,
			None
		};

	XVisualInfo *vi;
	Window root_win;
	XWindowAttributes win_attr;
	XSetWindowAttributes set_attr;
	XFontStruct *fixed;
	XColor black =
	{
		0, 0, 0, 0, 0, 0
	};

	if(( dpy = XOpenDisplay( NULL)) == NULL)
	{
		fprintf(stderr, "XOpenDisplay failed\n");
		return(1);
	}

	if(( vi = glXChooseVisual( dpy, DefaultScreen( dpy),
								vi_attr)) == NULL)
	{
		fprintf(stderr, "glXChooseVisual failed\n");
		XCloseDisplay(dpy);
		return(1);
	}

	root_win = RootWindow(dpy, vi->screen);

	XGetWindowAttributes(dpy, root_win, &win_attr);

	m_width= (fullscreen) ? win_attr.width: 640;
	m_height = (fullscreen) ? win_attr.height : 480;

	set_attr.border_pixel = 0;

	set_attr.colormap =
		XCreateColormap(dpy, root_win, vi->visual, AllocNone);

	set_attr.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
		ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

	set_attr.override_redirect = (( fullscreen) ? True : False);

	win =
		XCreateWindow(
				dpy, root_win, 0, 0, m_width, m_height, 0, vi->depth,
				InputOutput, vi->visual, CWBorderPixel | CWColormap |
				CWEventMask | CWOverrideRedirect, &set_attr);

	XStoreName(dpy, win, AppTitle);
	XMapWindow(dpy, win);

	if(fullscreen)
	{
		XGrabKeyboard( dpy, win, True, GrabModeAsync,
						GrabModeAsync, CurrentTime);
	}
	else
	{
		wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", True);
		XSetWMProtocols(dpy, win, &wmDelete, 1);
	}

	if(( ctx = glXCreateContext( dpy, vi, NULL, True)) == NULL)
	{
		fprintf(stderr, "glXCreateContext failed\n");
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
		return(1);
	}

	if(glXMakeCurrent( dpy, win, ctx) == False)
	{
		fprintf(stderr, "glXMakeCurrent failed\n");
		glXDestroyContext(dpy, ctx);
		XDestroyWindow(dpy, win);
		XCloseDisplay(dpy);
		return(1);
	}

	m_font = glGenLists(256);

	fixed = XLoadQueryFont(dpy, "-misc-fixed-medium-r-*-*-20-*-*-*-*-*-*-*");

	null_cursor = XCreateGlyphCursor(
		dpy, fixed->fid, fixed->fid, ' ', ' ', &black, &black);

	glXUseXFont(fixed->fid, 0, 256, m_font);

	XFreeFont(dpy, fixed);
#endif // HAVE_GL_GLX_H

	return(0);
}

/* timer structure */


/* timer query/reset routine */

float GDSParse_ogl::timer(struct htime *t, int reset)
{
	float delta;

#ifdef HAVE_GETTIMEOFDAY
	struct timeval offset;
	gettimeofday(&offset, NULL);

	delta = (float) (offset.tv_sec- t->start.tv_sec ) +
			(float) (offset.tv_usec - t->start.tv_usec) / 1e6;

	if(reset){
		t->start.tv_sec= offset.tv_sec;
		t->start.tv_usec = offset.tv_usec;
	}
#else

#ifdef HAVE_QUERYPERFORMANCECOUNTER
	LARGE_INTEGER offset;

	QueryPerformanceCounter(&offset);

	if(t->hfreq.QuadPart){
		delta = (float) (offset.QuadPart - t->start.QuadPart) /
			(float) t->hfreq.QuadPart;
	}

	if(reset){
		QueryPerformanceFrequency(&t->hfreq);
		QueryPerformanceCounter(&t->start);
	}
#else
#error "No suitable timer functions found"
#endif /* HAVE_QUERYPERFORMANCECOUNTER */
#endif /* HAVE_GETTIMEOFDAY */

	return(delta);
}

/* mouse handling routines */

void GDSParse_ogl::hide_mouse(void)
{
#ifdef HAVE_X11_XLIB_H
	XDefineCursor(dpy, win, null_cursor);
#else
	ShowCursor(FALSE);
#endif
}

void GDSParse_ogl::move_mouse(int x, int y)
{
#ifdef HAVE_X11_XLIB_H
	XWarpPointer(dpy, None, win, 0, 0, 0, 0, x, y);
#else
	POINT p;
	p.x = x;
	p.y = y;
	ClientToScreen(_hWnd, &p);
	SetCursorPos(p.x, p.y);
#endif
}
