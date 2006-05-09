/*
 * File: gdsparse_ogl.cpp
 * Author: Roger Light
 * Project: gdsto3d
 *
 * This is the OpenGL viewer specific implementation of the GDSParse class.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_STDARG_H
#  include <stdarg.h>
#endif
#ifdef HAVE_X11_KEYSYM_H
#  include <X11/keysym.h>
#endif
#ifdef HAVE_X11_XLIB_H
#  include <X11/Xlib.h>
#endif
#ifdef HAVE_GL_GLX_H
#  include <GL/glx.h>
#endif
#ifdef HAVE_GL_GLU_H
#  include <GL/glu.h>
#endif
#ifdef HAVE_GL_GL_H
#  include <GL/gl.h>
#endif

#ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
#endif
#ifdef HAVE_TIME_H
#  include <time.h>
#endif

#ifdef HAVE_WINDOWS_H
#  include <windows.h>
#endif

#include "gdsparse_ogl.h"
#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gdsoglviewer.h"
#include "gdsobject_ogl.h"

extern int verbose_output;

GDSParse_ogl::GDSParse_ogl(class GDSConfig *config, class GDSProcess *process) : GDSParse(config, process)
{
	_topcell = NULL;
	SetOutputOptions(false, false, true, false);
}

GDSParse_ogl::~GDSParse_ogl()
{
	if(_topcell){
		delete [] _topcell;
	}
}

class GDSObject *GDSParse_ogl::NewObject(char *Name)
{
	return new class GDSObject_ogl(Name);
}

void GDSParse_ogl::SetTopcell(char *topcell)
{
	if(topcell){
		if(_topcell){
			delete [] _topcell;
			_topcell = NULL;
		}
		_topcell = new char[strlen(topcell) + 1];
		strcpy(_topcell, topcell);
	}
}

void GDSParse_ogl::OutputHeader()
{
}

void GDSParse_ogl::OutputFooter()
{
}

/* application window title */

char *AppTitle = "GDSto3D";

/* external OS-dependant functions */

/* global data */

/* data initialization function */

int GDSParse_ogl::gl_data()
{
  _info = 1;
  
  _x = _rx = _vx = 0.0f;
  _y = _ry = _vy = 0.0f;
  _z =	     _vz = 0.0f;

  _speed_factor = 1;
  _xmin=_ymin=0;
  _xmax=_ymax=1;
  
  return( 0 );
}

/* gl initialization function */

int GDSParse_ogl::gl_init()
{
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
  glEnable( GL_LINE_SMOOTH );
  glEnable( GL_POLYGON_SMOOTH );
  glLineWidth( 2.0 );
  
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
  
  //glEnable(GL_LIGHTING);
  //glEnable(GL_LIGHT0);
  
  //glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT,GL_FASTEST);
  //glEnable(GL_DEPTH);
  glEnable(GL_DEPTH_TEST);
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  
  glPolygonMode(GL_FRONT_AND_BACK, render_mode);
  
  glNewList(1, GL_COMPILE);
  //glBegin(GL_TRIANGLES);
  Output(NULL, _topcell);
  //glEnd();
  glEndList();
  
  hide_mouse();
  move_mouse( _width / 2, _height / 2 );
  timer( &_mt, 1 );
  
  
  /* init view position, same as in gds2pov */
  if( _Objects){
    struct _Boundary *Boundary = _Objects->GetBoundary();
    
    float half_widthX = (Boundary->XMax - Boundary->XMin)/2;
    float half_widthY = (Boundary->YMax - Boundary->YMin)/2;
    float centreX = half_widthX + Boundary->XMin;
    float centreY = half_widthY + Boundary->YMin;
    
    float distance;
    if(half_widthX > half_widthY){
      distance = half_widthX * 1.8;
    }else{
      distance = half_widthY * 1.8;
    }
    
    _speed_factor = distance/20.0f;
    _xmin = Boundary->XMin;
    _xmax = Boundary->XMax;
    _ymin = Boundary->YMin;
    _ymax = Boundary->YMax;
    
    float XMod = _config->GetCameraPos()->XMod;
    float YMod = _config->GetCameraPos()->YMod;
    float ZMod = _config->GetCameraPos()->ZMod;
    
    switch(_config->GetCameraPos()->boundarypos){
    case bpCentre:
      // Default camera angle = 67.38
      // Half of this is 33.69
      // tan(33.69) = 0.66666 = 1/1.5
      // Make it slightly larger so that we have a little bit of a border: 1.5+20% = 1.8
      
      _x = centreX*XMod;
      _y = centreY*YMod;
      _z = distance*ZMod;
      break;
    case bpTopLeft:
      _x = Boundary->XMin*XMod;
      _y = Boundary->YMax*YMod;
      _z = distance*ZMod;
      break;
    case bpTopRight:
      _x = Boundary->XMax*XMod; 
      _y = Boundary->YMax*YMod;
      _z = distance*ZMod;
      break;
    case bpBottomLeft:
      _x = Boundary->XMin*XMod;
      _y = Boundary->YMin*YMod;
      _z = distance*ZMod;
      break;
    case bpBottomRight:
      _x = Boundary->XMax*XMod;
      _y = Boundary->YMin*YMod;
      _z = distance*ZMod;
      break;
    }
  }   /* end init view position */
  
  return( 0 );
}

/* window drawing function */
void GDSParse_ogl::gl_draw()
{
	GLfloat M[16],G[16];
	GLfloat R1x,R1y,R1z;
	long objectid=0;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	//glClear( GL_COLOR_BUFFER_BIT );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	if( _fps ){
	        /*change angle according to velocity */
		_ry += _vy / _fps;
		_rx += _vx / _fps;

		/* slow velocity down ??*/
		if( _fps > 4.0f ){
			_vy *= 1.0f - 4.0f / _fps;
			_vx *= 1.0f - 4.0f / _fps;
		}else{
			_vy = 0.0f;
			_vx = 0.0f;
		}
	}


	/* make sure angles are in [-180,+180] */
	if( _ry >  180.0f ) _ry += -360.0f;
	if( _ry < -180.0f ) _ry +=  360.0f;

	if( _rx >  180.0f ) _rx += -360.0f;
	if( _rx < -180.0f ) _rx +=  360.0f;


	//FIXME glRotatef( _ry, 0.0f, 1.0f, 0.0f );
	glRotatef( _ry, 0.0f, 0.0f, 1.0f );
	glGetFloatv( GL_MODELVIEW_MATRIX, M );

	/* save vector so we can calculate the inverse rotation later */
	R1x=M[0];R1y=M[4];R1z=M[8];
	glRotatef( _rx, M[0], M[4], M[8] );
	glGetFloatv( GL_MODELVIEW_MATRIX, M );
	glFlush();

	if( _fps ){
		_x -= M[2]  * _vz / _fps;
		_y -= M[6]  * _vz / _fps;
		_z -= M[10] * _vz / _fps;
	}

	/* FIXME: something is wrong, without the next chunk of code, the  program starts up much faster, once it's up
	   it doesn't make a difference */
	{
	  /* add an estimate for zFar, so that the whole obeject is always in view */
	  glMatrixMode(GL_PROJECTION); 
	  glLoadIdentity(); 
	  {
	    GLfloat tmpz;
	    GLfloat x,y,z;

	    /* distance to the bounding box, use larger value...
	       if we would know the direction were are looking in, we could use the correct distance, but this works */
	    x = (_x-_xmin);     
	    if ((_xmax-_x) > x) x = _xmax-_x;
	    
	    y = (_y-_ymin);
	    if ((_ymax-_y) > y) y = _ymax-_y;
	    
	    /* FIXME: should use zmax and zmin instead of a default value of 50 */
	    z = (_z+50);
	    if ((50-_z) > z) z = 50-_z; 
	    
	    tmpz = x+y+z; /* correct would be \sqrt(x*x+y*y+z*z)+\epsilon, but this should be faster and as good */
	    if(tmpz<10.0) tmpz = 10.0; /* not sure if neccessary */
	    
	    gluPerspective(50.0f , 1.0f, 1.0f,tmpz);  /* set zFar */
	  }
	  glMatrixMode( GL_MODELVIEW ); /* back to the model view  */
	}
	

	if( _vz < -40.0f*_speed_factor ) _vz = -40.0f*_speed_factor;
	if( _vz >  40.0f*_speed_factor ) _vz =  40.0f*_speed_factor;

/*
	if( _x < -6.0f ) _x += 6.0f;
	if( _x >  6.0f ) _x -= 6.0f;
	if( _z < -6.0f ) _z += 6.0f;
	if( _z >  6.0f ) _z -= 6.0f;
	if( _y < -0.7f ) _y = -0.7f;
	if( _y >  0.7f ) _y =  0.7f;
*/
	glTranslatef( -_x, -_y, -_z );
	glGetFloatv( GL_MODELVIEW_MATRIX, M );

	glCallList(1);

	/* figure out look at statement: calculate inverse transformation and apply to (0,0,-1) */
	glLoadIdentity();
	glTranslatef( _x, _y, _z );
	glRotatef( -(_rx), R1x, R1y, R1z );
	glRotatef( -(_ry), 0.0f, 0.0f, 1.0f );
	glGetFloatv( GL_MODELVIEW_MATRIX, G );

	if( _fps && _info )
	{
		gl_printf( 0.1f, 1.0f, 0.1f, 0.4f, _width - 114, _height - 40,
				   _font, "fps: %5.1f", _fps );
 		gl_printf( 0.1f, 1.0f, 0.1f, 0.4f, _width - 420, _height - 40,
 				   _font, "location:"  );
 		gl_printf( 0.1f, 1.0f, 0.1f, 0.4f, _width - 320, _height - 40,
			   _font, " %5.1f %5.1f %5.1f",_x,_y,-(_z)  ); /*  -z because of povrays coordinate system? */
 		gl_printf( 0.1f, 1.0f, 0.1f, 0.4f, _width - 420, _height - 60,
 				   _font, "look_at:"  );
 		gl_printf( 0.1f, 1.0f, 0.1f, 0.4f, _width - 320, _height - 60,
 				   _font, " %5.1f %5.1f %5.1f", -G[8]+G[12],-G[9]+G[13],+G[10]-G[14]  ); 

		if( _fps < 20.0f ){
			glDisable( GL_LINE_SMOOTH );
			glDisable( GL_POINT_SMOOTH );
			glDisable( GL_POLYGON_SMOOTH );
		}
	}

	glFinish();

	_frames++;

	if( timer( &_tv, 0 ) >= 0.2f )
	{
		_fps = (GLfloat) _frames / timer( &_tv, 1 );
		_frames = 0;
	}
}

void GDSParse_ogl::gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
				GLint x, GLint y, GLuint font, const char *format, ... )
{
	va_list argp;
	char text[256];

	va_start( argp, format );
	vsprintf( text, format, argp );
	va_end( argp );

	glMatrixMode( GL_PROJECTION );
	glPushMatrix();

		glLoadIdentity();
		gluOrtho2D( 0.0, (GLdouble) _width,
					0.0, (GLdouble) _height );

		glMatrixMode( GL_MODELVIEW );
		glLoadIdentity();

		glColor4f( red, green, blue, alpha );
		glRasterPos2i( x, y );
		glListBase( font );
		glCallLists( strlen( text ), GL_UNSIGNED_BYTE, text );

	glMatrixMode( GL_PROJECTION );
	glPopMatrix();
}
/* window resizing function */

void GDSParse_ogl::gl_resize( void )
{
	glViewport( 0, 0, _width, _height );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 50.0, (GLdouble) _width / _height, 0.1, 100.0 );

	_fps = 0.0f;
	_frames = 0;
	timer( &_tv, 1 );
}

/* event handling function */

void GDSParse_ogl::gl_event( int event, int data, int xpos, int ypos )
{
	if( event == 0 )	/* mouse button down */
	{
		if( data == 0 ) /* left button */
		{
			_vz +=  5.0f*_speed_factor;
		}

		if( data == 1 ) /* right button */
		{
			_vz += -5.0f*_speed_factor;
		}
	}

	if( event == 1 )	/* mouse button up */
	{
		_vz = 0.0f;
	}

	if( event == 2 )	/* mouse move */
	{
		_vy +=  128.0f * _speed_factor * (GLfloat) ( xpos - _width  / 2 ) / _width;
		_vx -=  128.0f * _speed_factor * (GLfloat) ( ypos - _height / 2 ) / _height;

		if( timer( &_mt, 0 ) > 0.05 )
		{
			timer( &_mt, 1 );
			move_mouse( _width / 2, _height / 2 );
		}
	}

	if( event == 3 )	/* key down */
	{
	}

	if( event == 4 )	/* key up */
	{
		if( data == ' ' )
		{
			_info ^= 1;
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

	if( gl_data() )
{
		return( 1 );
	}

	fullscreen = 0;

	do{
		_modeswitch  = 0;
		fullscreen ^= 1;

		if( glx_init( fullscreen ) ){
			fprintf( stderr, "glx_init failed\n" );
			return( 1 );
		}

		if( gl_init() ){
			fprintf( stderr, "gl_init failed\n" );
			return( 1 );
		}

		gl_resize();

		_run = 1;

		while( _run ){
			if( _active ){
				gl_draw();
				glXSwapBuffers( dpy, win );
			}else{
				XPeekEvent( dpy, &event );
			}

			while( XPending( dpy ) ){
				XNextEvent( dpy, &event );

				switch( event.type ){
					case ButtonPress:
					{
						int x = event.xmotion.x,
							y = event.xmotion.y;

						switch( event.xbutton.button )
						{
							case Button1: gl_event( 0, 0, x, y ); break;
							case Button3: gl_event( 0, 1, x, y ); break;
						}

						break;
					}

					case ButtonRelease:
					{
						int x = event.xmotion.x,
							y = event.xmotion.y;

						switch( event.xbutton.button )
						{
							case Button1: gl_event( 1, 0, x, y ); break;
							case Button3: gl_event( 1, 1, x, y ); break;
						}

						break;
					}

					case MotionNotify:
			{
						int x = event.xmotion.x,
							y = event.xmotion.y;

						switch( event.xbutton.button )
						{
							case Button1: gl_event( 2,  0, x, y ); break;
							case Button3: gl_event( 2,  1, x, y ); break;
							default:	  gl_event( 2, -1, x, y ); break;
						}

						break;
					}

					case KeyPress:
					{
						break;
					}

					case KeyRelease:
					{
						int key = XLookupKeysym( &event.xkey, 0 );

						switch( key )
						{
							case XK_Tab:
								_modeswitch = 1;

							case XK_Escape:
								_run = 0;
								break;

							default:

								gl_event( 4, key, -1, -1 );
								break;
						}

						break;
					}

					case UnmapNotify: _active = 0; break;
					case   MapNotify: _active = 1; break;

					case ConfigureNotify:
					{
						_width  = event.xconfigure.width;
						_height = event.xconfigure.height;
						gl_resize();
						break;
					}

					case ClientMessage:	
					{
						if( event.xclient.data.l[0] == (int) wmDelete )
						{
							_active = _run = 0;
						}
						break;
					}

					case ReparentNotify: break;

					default:
					{
						printf( "caught unknown event, type %d\n",
								event.type );
						break;
					}
				}
			}
		}

		glXMakeCurrent( dpy, None, NULL );
		glXDestroyContext( dpy, ctx );
		XDestroyWindow( dpy, win );
		XCloseDisplay( dpy );
	}
	while( _modeswitch );
#endif // HAVE_GL_GLX_H

	return( 0 );
}

int GDSParse_ogl::glx_init( int fullscreen )
{
#ifdef HAVE_GL_GLX_H
	int vi_attr[] =
		{
			GLX_RGBA,
			GLX_DOUBLEBUFFER,
			GLX_RED_SIZE,	   4, 
			GLX_GREEN_SIZE,	 4, 
			GLX_BLUE_SIZE,	  4, 
			GLX_DEPTH_SIZE,	16,
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

	if( ( dpy = XOpenDisplay( NULL ) ) == NULL )
	{
		fprintf( stderr, "XOpenDisplay failed\n" );
		return( 1 );
	}

	if( ( vi = glXChooseVisual( dpy, DefaultScreen( dpy ),
								vi_attr ) ) == NULL )
	{
		fprintf( stderr, "glXChooseVisual failed\n" );
		XCloseDisplay( dpy );
		return( 1 );
	}

	root_win = RootWindow( dpy, vi->screen );

	XGetWindowAttributes( dpy, root_win, &win_attr );

	_width  = ( fullscreen ) ? win_attr.width  : 640;
	_height = ( fullscreen ) ? win_attr.height : 480;

	set_attr.border_pixel = 0;

	set_attr.colormap =
		XCreateColormap( dpy, root_win, vi->visual, AllocNone );

	set_attr.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
		ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

	set_attr.override_redirect = ( ( fullscreen ) ? True : False );

	win = 
		XCreateWindow(
				dpy, root_win, 0, 0, _width, _height, 0, vi->depth,
				InputOutput, vi->visual, CWBorderPixel | CWColormap |
				CWEventMask | CWOverrideRedirect, &set_attr );

	XStoreName( dpy, win, AppTitle );
	XMapWindow( dpy, win );

	if( fullscreen )
	{
		XGrabKeyboard(  dpy, win, True, GrabModeAsync,
						GrabModeAsync, CurrentTime );
	}
	else
	{
		wmDelete = XInternAtom( dpy, "WM_DELETE_WINDOW", True );
		XSetWMProtocols( dpy, win, &wmDelete, 1 );
	}

	if( ( ctx = glXCreateContext( dpy, vi, NULL, True ) ) == NULL )
	{
		fprintf( stderr, "glXCreateContext failed\n" );
		XDestroyWindow( dpy, win );
		XCloseDisplay( dpy );
		return( 1 );
	}

	if( glXMakeCurrent( dpy, win, ctx ) == False )
	{
		fprintf( stderr, "glXMakeCurrent failed\n" );
		glXDestroyContext( dpy, ctx );
		XDestroyWindow( dpy, win );
		XCloseDisplay( dpy );
		return( 1 );
	}

	_font = glGenLists( 256 );

	fixed = XLoadQueryFont(
		dpy, "-misc-fixed-medium-r-*-*-20-*-*-*-*-*-*-*" );

	null_cursor = XCreateGlyphCursor(
		dpy, fixed->fid, fixed->fid, ' ', ' ', &black, &black );

	glXUseXFont( fixed->fid, 0, 256, _font );

	XFreeFont( dpy, fixed );
#endif // HAVE_GL_GLX_H

	return( 0 );
}

/* timer structure */


/* timer query/reset routine */

float GDSParse_ogl::timer( struct htime *t, int reset )
{
	float delta;

#ifdef HAVE_GETTIMEOFDAY
	struct timeval offset;
	gettimeofday( &offset, NULL );

	delta = (float) ( offset.tv_sec  - t->start.tv_sec  ) +
			(float) ( offset.tv_usec - t->start.tv_usec ) / 1e6;

	if( reset )
	{
		t->start.tv_sec  = offset.tv_sec;
		t->start.tv_usec = offset.tv_usec;
	}
#else
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
#endif

	return( delta );
}

/* mouse handling routines */

void GDSParse_ogl::hide_mouse( void )
{
#ifdef HAVE_X11_XLIB_H
	XDefineCursor( dpy, win, null_cursor );
#else
	ShowCursor(FALSE);
#endif
}

void GDSParse_ogl::move_mouse( int x, int y )
{
#ifdef HAVE_X11_XLIB_H
	XWarpPointer( dpy, None, win, 0, 0, 0, 0, x, y );
#else
	POINT p;
	p.x = x;
	p.y = y;
	ClientToScreen(_hWnd, &p);
	SetCursorPos(p.x, p.y);
#endif
}


