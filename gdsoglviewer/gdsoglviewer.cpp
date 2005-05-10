#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_GL_GL_H
#  include <GL/gl.h>
#endif

#ifdef HAVE_WINDOWS_H
#  include <windows.h>
#endif

#include "gdsoglviewer.h"
#include "gdsparse_ogl.h"
#include "gds_globals.h"
#include "process_cfg.h"

#define GDSOGLVIEWER_VERSION "0.8"

#ifdef WIN32
LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );
int WINAPI RealWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow );
class GDSParse_ogl *parser;
#endif

/*extern int verbose_output;
extern bool quiet_output;
extern */
int render_mode;

void printusage()
{
		printf("gdsoglviewer  version %s\n", GDSOGLVIEWER_VERSION);
		printf("Copyright (C) 2004,2005 by Roger Light\nhttp://www.atchoo.org/gdsto3d/\n\n");
		printf("gdsoglviewer comes with ABSOLUTELY NO WARRANTY.  You may distribute gdsoglviewer freely\nas described in the readme.txt distributed with this file.\n\n");
		printf("gdsoglviewer is a program for converting a GDS2 file to a number of 3D formats.\n\n");
		printf("Usage: gdsoglviewer input.gds [-c config.txt] [-m solid|wire] [-p process.txt] [-q] [-t topcell] [-v]\n\n");
		printf("Options\n");
		printf(" -c\t\tSpecify config file\n");
		printf(" -m\t\tSpecify solid or wireframe mode\n");
		printf(" -p\t\tSpecify process file\n");
		printf(" -q\t\tQuiet output\n");
		printf(" -t\t\tSpecify top cell name\n");
		printf(" -v\t\tVerbose output\n\n");
		printf("See http://www.atchoo.org/gdsto3d/ for updates.\n");
}

#ifdef WIN32
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
#else
int main(int argc, char *argv[])
#endif
{
	verbose_output = 1;
	//render_mode = GL_LINE;
	render_mode = GL_FILL;

	char *processfile=NULL;
	char *topcell=NULL;

#ifdef WIN32
	char *gdsfile = "RF1_Top.gds";
	//char *gdsfile = "polytest.gds";
	char *configfile = "config.txt";
#else
	if(argc<3 || argc>13){
		printf("Error: Invalid number of arguments.\n\n");
		printusage();
		return 1;
	}

	char *gdsfile=argv[1];

	char *configfile=NULL;

	for(int i=3; i<argc; i++){
		if(argv[i][0] == '-'){
			if(strncmp(argv[i], "-c", strlen("-c"))==0){

				if(i==argc-1){
					printf("Error: -c switch given but no config file specified.\n\n");
					printusage();
					return 1;
				}else{
					configfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-m", strlen("-m"))==0){
				if(i==argc-1){
					printf("Error: -m switch given but no render mode specified.\n\n");
					printusage();
					return 1;
				}else{
					if(strncmp(argv[i+1], "solid", strlen("solid"))==0){
						render_mode = GL_FILL;
					}else if(strncmp(argv[i+1], "wire", strlen("wire"))==0){
						render_mode = GL_LINE;
					}else{
						printf("Error: Invalid render mode.\n\n");
						printusage();
						return 1;
					}
				}
			}else if(strncmp(argv[i], "-p", strlen("-p"))==0){
				if(i==argc-1){
					printf("Error: -p switch given but no process file specified.\n\n");
					printusage();
					return 1;
				}else{
					processfile = argv[i+1];
				}
			}else if(strncmp(argv[i], "-q", strlen("-q"))==0){
				verbose_output--;
			}else if(strncmp(argv[i], "-t", strlen("-t"))==0){
				if(i==argc-1){
					printf("Error: -t switch given but no top cell specified.\n\n");
					printusage();
					return 1;
				}else{
					topcell = argv[i+1];
				}
			}else if(strncmp(argv[i], "-v", strlen("-v"))==0){
				verbose_output++;
			}else{
				printusage();
				return 1;
			}
		//}else{
		// Assume it is a process/config file specified on a previous arg
		}
	}
#endif // WIN32

	class GDSConfig *config=NULL;

	if(configfile){
		config = new GDSConfig(configfile);
	}else{
		config = new GDSConfig(); // Start with default positions
	}
	if(!config){
		fprintf(stderr, "Error: Out of memory.\n");
		return -1;
	}else if(!config->IsValid()){
		fprintf(stderr, "Error: %s is not a valid config file.\n", configfile);
		delete config;
		return -1;
	}

	class GDSProcess *process=NULL;
	/*
	** Order of precedence for process.txt:
	** -p switch (given as an argument to this function)
	** Specified in config file
	** Use process.txt if none specified.
	*/
	if(processfile == NULL){
		if(config->GetProcessFile()!=NULL){
			processfile = config->GetProcessFile();
		}else{
			processfile = new char[13];
			strncpy(processfile, "process.txt", 13);
		}
	}
	process = new GDSProcess(processfile);
	process->Parse(processfile);
	if(!process){
		fprintf(stderr, "Error: Out of memory.\n");
		delete config;
		return -1;
	}else if(!process->IsValid()){
		fprintf(stderr, "Error: %s is not a valid process file\n");
		delete config;
		delete process;
		return -1;
	}else if(process->LayerCount()==0){
		fprintf(stderr, "Error: No layers found in \"%s\".\n", processfile);
		delete config;
		delete process;
		return -1;
	}

	FILE *iptr = fopen(gdsfile, "rb");
	if(iptr){
		class GDSParse_ogl *Parser = new class GDSParse_ogl(config, process);
		if(!Parser->Parse(iptr)){
#ifdef WIN32
			parser = Parser;
			RealWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
#else
			Parser->Output(NULL, topcell, false, true, false);
#endif
		}

		fclose(iptr);
		delete Parser;
		delete config;
		delete process;
	}else{
		fprintf(stderr, "Error: Unable to open %s.\n", gdsfile);
		delete config;
		delete process;
		return -1;
	}
	return 0;
}

#ifdef WIN32
int WINAPI RealWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow )
{
    char ClassName[] = "opengl demo";
    int fullscreen;
    WNDCLASSEX wcx;
    DWORD dwStyle;
    MSG msg;

    if( parser->gl_data() )
    {
        //WinPerror( "gl_data" );
        return( 0 );
    }

    wcx.cbSize          = sizeof( WNDCLASSEX );
    wcx.style           = 0;
    wcx.lpfnWndProc     = (WNDPROC) WindowProc;
    wcx.cbClsExtra      = 0;
    wcx.cbWndExtra      = 0;
    wcx.hInstance       = hInstance;
    wcx.hIcon           = NULL;
    wcx.hCursor         = LoadCursor( NULL, IDC_ARROW );
    wcx.hbrBackground   = (HBRUSH) COLOR_BACKGROUND;
    wcx.lpszMenuName    = NULL;
    wcx.lpszClassName   = ClassName;
    wcx.hIconSm         = NULL;

    if( ! RegisterClassEx( &wcx ) )
    {
        //WinPerror( "RegisterClassEx" );
        return( 0 );
    }

    fullscreen = 0;

    do
    {
        parser->_modeswitch  = 0;
        fullscreen ^= 1;

        if( fullscreen )
        {
            if( ! ( parser->_hDC = GetDC( NULL ) ) )
            {
             //   WinPerror( "GetDC" );
                return( 0 );
            }

            parser->_width  = GetDeviceCaps( parser->_hDC, HORZRES );
            parser->_height = GetDeviceCaps( parser->_hDC, VERTRES );

            ReleaseDC( NULL, parser->_hDC );

            dwStyle = WS_POPUP;
        }
        else
        {
            parser->_width  = 640;
            parser->_height = 480;

            dwStyle = WS_OVERLAPPEDWINDOW;
        }

        if( ! ( parser->_hWnd = CreateWindow( ClassName, "FIXME", dwStyle,
                        CW_USEDEFAULT, CW_USEDEFAULT, parser->_width, parser->_height,
                        NULL, NULL, hInstance, NULL ) ) )
        {
            //WinPerror( "CreateWindowEx" );
            return( 0 );
        }

        ShowWindow( parser->_hWnd, nCmdShow );

        parser->_run = 1;

        while( parser->_run )
        {
            if( parser->_active )
            {
                parser->gl_draw();
		//parser->Output(NULL, NULL, false, true, false);
                SwapBuffers( parser->_hDC );
            }
            else
            {
                GetMessage( &msg, NULL, 0, 0 );
            }

            while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
            {
                if( msg.message == WM_QUIT )
                {
                    parser->_run = 0;
                    break;
                }

                TranslateMessage( &msg );
                DispatchMessage( &msg ); 
            }
        }

        wglMakeCurrent( NULL, NULL );
        if( parser->_hDC ) ReleaseDC( parser->_hWnd, parser->_hDC );
        if( parser->_hRC ) wglDeleteContext( parser->_hRC );
        DestroyWindow( parser->_hWnd );
    }
    while( parser->_modeswitch );

    return( msg.wParam );
}

/* the window procedure itself */

LRESULT CALLBACK WindowProc( HWND hWnd, UINT uMsg, WPARAM wParam,
                             LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_CREATE:
        {
            PIXELFORMATDESCRIPTOR pfd =
            {
                sizeof( PIXELFORMATDESCRIPTOR ),
                1,
                    PFD_DRAW_TO_WINDOW |
                    PFD_SUPPORT_OPENGL |
                    PFD_DOUBLEBUFFER,
                PFD_TYPE_RGBA,
                16,
                0, 0, 0, 0, 0, 0,
                0,
                0,
                0,
                0, 0, 0, 0,
                16,
                0,
                0,
                PFD_MAIN_PLANE,
                0,
                0, 0, 0
            };
            int pixelformat, font;

            parser->_hRC = NULL;

            if( ! ( parser->_hDC = GetDC( hWnd ) ) )
            {
                //WinPerror( "GetDC" );
                PostQuitMessage( 0 );
                break;
            }

            if( ! ( pixelformat = ChoosePixelFormat( parser->_hDC, &pfd ) ) )
            {
                //WinPerror( "ChoosePixelFormat" );
                PostQuitMessage( 0 );
                break;
            }

            if( ! SetPixelFormat( parser->_hDC, pixelformat, &pfd ) )
            {
                //WinPerror( "SetPixelFormat" );
                PostQuitMessage( 0 );
                break;
            }

            if( ! ( parser->_hRC = wglCreateContext( parser->_hDC ) ) )
            {
                //WinPerror( "wglCreateContext" );
                PostQuitMessage( 0 );
                break;
            }
            
            if( ! wglMakeCurrent( parser->_hDC, parser->_hRC ) )
            {
                //WinPerror( "wglMakeCurrent" );
                PostQuitMessage( 0 );
                break;
            }

            font = glGenLists( 255 );

            {
                HFONT courier = CreateFont( 20, 0, 0, 0, FW_MEDIUM, FALSE,
                                FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS,
                                CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY,
                                FF_DONTCARE | DEFAULT_PITCH, "Courier New" );

                SelectObject( parser->_hDC, courier );

                if( ! wglUseFontBitmaps( parser->_hDC, 1, 255, font ) )
                {
                    //WinPerror( "wglUseFontBitmaps" );
                    PostQuitMessage( 0 );
                }
            }

            if( parser->gl_init() )
            {
                //WinPerror( "gl_init" );
                PostQuitMessage( 0 );
                break;
            }

            parser->gl_resize();
            break;
        }

        case WM_LBUTTONDOWN:
        {
            parser->gl_event( 0, 0, LOWORD(lParam), HIWORD(lParam) );
            break;
        }

        case WM_RBUTTONDOWN:
        {
            parser->gl_event( 0, 1, LOWORD(lParam), HIWORD(lParam) );
            break;
        }

        case WM_LBUTTONUP:
        {
            parser->gl_event( 1, 0, LOWORD(lParam), HIWORD(lParam) );
            break;
        }

        case WM_RBUTTONUP:
        {
            parser->gl_event( 1, 1, LOWORD(lParam), HIWORD(lParam) );
            break;
        }

        case WM_MOUSEMOVE:
        {
            switch( wParam )
            {
                case MK_LBUTTON:

                    parser->gl_event( 2,  0, LOWORD(lParam), HIWORD(lParam) );
                    break;

                case MK_RBUTTON:

                    parser->gl_event( 2,  1, LOWORD(lParam), HIWORD(lParam) );
                    break;

                default:

                    parser->gl_event( 2, -1, LOWORD(lParam), HIWORD(lParam) );
                    break;
            }

            break;
        }

        case WM_KEYDOWN:
        {
            break;
        }

        case WM_KEYUP:
        {
            switch( wParam )
            {
                case VK_TAB:

                    parser->_modeswitch = 1;

                case VK_ESCAPE:
                    
                    PostQuitMessage( 1 );
                    break;

                default:

                    parser->gl_event( 4, wParam, -1, -1 );
                    break;
            }

            break;
        }

        case WM_SIZE:
        {
            switch( wParam )
            {
                case SIZE_MINIMIZED: parser->_active = 0; break;
                case SIZE_MAXIMIZED: parser->_active = 1; break;
                case SIZE_RESTORED:  parser->_active = 1; break;
            }

            parser->_width  = LOWORD( lParam );
            parser->_height = HIWORD( lParam );
            parser->gl_resize();
            break;
        }

        case WM_CLOSE:
        {
            PostQuitMessage( 1 );
            break;
        }

        case WM_SYSCOMMAND:
        {
            switch( wParam )
            {
                case SC_MONITORPOWER:
                case SC_SCREENSAVE:

                    return 0;
            }
        }

        default:

            return( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
    }

    return( 0 );
}

#endif // WIN32

