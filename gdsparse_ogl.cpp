#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gdsparse.h"
#include "config_cfg.h"
#include "process_cfg.h"
#include "gds_globals.h"
#include "gds2pov.h"

extern int verbose_output;

void GDSParse::OutputOGLHeader()
{
	fprintf(optr, "#ifdef WIN32\n#include <windows.h>\n#endif\n");
	fprintf(optr, "#include <GL/gl.h>\n");
	fprintf(optr, "#include <GL/glu.h>\n");
	fprintf(optr, "#include <stdio.h>\n");
	fprintf(optr, "\nvoid gds_draw()\n{\n");
}

void GDSParse::OutputOGLFooter()
{
	fprintf(optr, "}\n");
}

#include <X11/keysym.h>
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/glu.h>
#include <GL/gl.h>

#include <sys/time.h>
#include <stdio.h>
#include <time.h>


/* application window title */

char *AppTitle = "GDSto3D";

/* external OS-dependant functions */

/* global data */

/* data initialization function */

int GDSParse::gl_data( void )
{
    info = 1;

    x = rx = vx = 0.0f;
    y = ry = vy = 0.0f;
    z =      vz = 0.0f;

    return( 0 );
}

/* gl initialization function */

int GDSParse::gl_init( void )
{
    glEnable( GL_LINE_SMOOTH );
    glLineWidth( 2.0 );

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glEnable(GL_DEPTH);
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

    hide_mouse();
    move_mouse( width / 2, height / 2 );
    timer( &mt, 1 );

    return( 0 );
}

/* window drawing function */

void GDSParse::gl_draw(class GDSObject *Object)
{
    GLfloat M[16];
	long objectid=0;

    //glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClear( GL_COLOR_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    if( fps )
    {
        ry += vy / fps;
        rx += vx / fps;

        if( fps > 4.0f )
        {
            vy *= 1.0f - 4.0f / fps;
            rx *= 1.0f - 4.0f / fps;
        }
        else
        {
            vy = 0.0f;
            rx = 0.0f;
        }
    }

    /*if( ry >  180.0f ) ry += -360.0f;
    if( ry < -180.0f ) ry +=  360.0f;
    */

    glRotatef( ry, 0.0f, 1.0f, 0.0f );
    glGetFloatv( GL_MODELVIEW_MATRIX, M );
    glRotatef( rx, M[0], M[4], M[8] );
    glGetFloatv( GL_MODELVIEW_MATRIX, M );
    glFlush();

    if( fps )
    {
        x -= M[2]  * vz / fps;
        y -= M[6]  * vz / fps;
        z -= M[10] * vz / fps;
    }

    if( vz < -40.0f ) vz = -40.0f;
    if( vz >  40.0f ) vz =  40.0f;

    /*if( x < -6.0f ) x += 6.0f;
    if( x >  6.0f ) x -= 6.0f;
    if( z < -6.0f ) z += 6.0f;
    if( z >  6.0f ) z -= 6.0f;
    if( y < -0.7f ) y = -0.7f;
    if( y >  0.7f ) y =  0.7f;
*/
    glTranslatef( -x, -y, -z );

	RecursiveOutput(Object, NULL, 0.0, 0.0, &objectid);

    if( fps && info )
    {
        gl_printf( 0.1f, 1.0f, 0.1f, 0.4f, width - 114, height - 40,
                   font, "%5.1f fps", fps );

        if( fps < 20.0f ) glDisable( GL_LINE_SMOOTH );
    }

    glFinish();

    frames++;

    if( timer( &tv, 0 ) >= 0.2f )
    {
        fps = (GLfloat) frames / timer( &tv, 1 );
        frames = 0;
    }
}

void GDSParse::gl_printf( GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha,
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
        gluOrtho2D( 0.0, (GLdouble) width,
                    0.0, (GLdouble) height );

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

void GDSParse::gl_resize( void )
{
    glViewport( 0, 0, width, height );

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 50.0, (GLdouble) width / height, 0.1, 100.0 );

    fps = 0.0f;
    frames = 0;
    timer( &tv, 1 );
}

/* event handling function */

void GDSParse::gl_event( int event, int data, int xpos, int ypos )
{
    if( event == 0 )    /* mouse button down */
    {
        if( data == 0 ) /* left button */
        {
            //vx =  40.0f;
            vx +=  10.0f;
        }

        if( data == 1 ) /* right button */
        {
            //vx = -40.0f;
            vx += -10.0f;
        }
    }

    if( event == 1 )    /* mouse button up */
    {
        //vx = 0.0f;
    }

    if( event == 2 )    /* mouse move */
    {
        vy += 256.0f * (GLfloat) ( xpos - width  / 2 ) / width;
        vz -=  16.0f * (GLfloat) ( ypos - height / 2 ) / height;

        if( timer( &mt, 0 ) > 0.05 )
        {
            timer( &mt, 1 );
            move_mouse( width / 2, height / 2 );
        }
    }

    if( event == 3 )    /* key down */
    {
    }

    if( event == 4 )    /* key up */
    {
        if( data == ' ' )
        {
            info ^= 1;
        }
    }
}

/* external gl data */

/* variables associated with the window */


/* declaration of the GLX initialization function */

/* program entry point */

int GDSParse::gl_main(class GDSObject *Object)
{
    int fullscreen;
    XEvent event;

    if( gl_data() ){
        fprintf( stderr, "gl_data failed\n" );
        return( 1 );
    }

    fullscreen = 0;

    do{
        modeswitch  = 0;
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

        run = 1;

        while( run ){
            if( active ){
                gl_draw(Object);
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
                            default:      gl_event( 2, -1, x, y ); break;
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
                                modeswitch = 1;

                            case XK_Escape:
                                run = 0;
                                break;

                            default:

                                gl_event( 4, key, -1, -1 );
                                break;
                        }

                        break;
                    }

                    case UnmapNotify: active = 0; break;
                    case   MapNotify: active = 1; break;

                    case ConfigureNotify:
                    {
                        width  = event.xconfigure.width;
                        height = event.xconfigure.height;
                        gl_resize();
                        break;
                    }

                    case ClientMessage:    
                    {
                        if( event.xclient.data.l[0] == (int) wmDelete )
                        {
                            active = run = 0;
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
    while( modeswitch );

    return( 0 );
}

int GDSParse::glx_init( int fullscreen )
{
    int vi_attr[] =
        {
            GLX_RGBA,
            GLX_DOUBLEBUFFER,
            GLX_RED_SIZE,       4, 
            GLX_GREEN_SIZE,     4, 
            GLX_BLUE_SIZE,      4, 
            GLX_DEPTH_SIZE,    16,
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

    width  = ( fullscreen ) ? win_attr.width  : 640;
    height = ( fullscreen ) ? win_attr.height : 480;

    set_attr.border_pixel = 0;

    set_attr.colormap =
        XCreateColormap( dpy, root_win, vi->visual, AllocNone );

    set_attr.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask |
        ButtonReleaseMask | PointerMotionMask | StructureNotifyMask;

    set_attr.override_redirect = ( ( fullscreen ) ? True : False );

    win = 
        XCreateWindow(
                dpy, root_win, 0, 0, width, height, 0, vi->depth,
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

    font = glGenLists( 256 );

    fixed = XLoadQueryFont(
        dpy, "-misc-fixed-medium-r-*-*-20-*-*-*-*-*-*-*" );

    null_cursor = XCreateGlyphCursor(
        dpy, fixed->fid, fixed->fid, ' ', ' ', &black, &black );

    glXUseXFont( fixed->fid, 0, 256, font );

    XFreeFont( dpy, fixed );

    return( 0 );
}

/* timer structure */


/* timer query/reset routine */

float GDSParse::timer( struct htime *t, int reset )
{
    float delta;
    struct timeval offset;

    gettimeofday( &offset, NULL );

    delta = (float) ( offset.tv_sec  - t->start.tv_sec  ) +
            (float) ( offset.tv_usec - t->start.tv_usec ) / 1e6;

    if( reset )
    {
        t->start.tv_sec  = offset.tv_sec;
        t->start.tv_usec = offset.tv_usec;
    }

    return( delta );
}

/* mouse handling routines */

void GDSParse::hide_mouse( void )
{
    XDefineCursor( dpy, win, null_cursor );
}

void GDSParse::move_mouse( int x, int y )
{
    XWarpPointer( dpy, None, win, 0, 0, 0, 0, x, y );
}

