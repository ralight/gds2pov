#ifndef _GDSOGLVIEWER_H
#define _GDSOGLVIEWER_H

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#ifdef HAVE_WINDOWS_H
#  include <windows.h>
#endif

extern int render_mode;

#ifdef WIN32
LRESULT CALLBACK WindowProc( HWND, UINT, WPARAM, LPARAM );
int WINAPI RealWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow );
#endif


#endif
