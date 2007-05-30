# Check for header files
# ======================
AC_CHECK_HEADERS([stdarg.h X11/keysym.h X11/Xlib.h GL/glx.h sys/time.h time.h windows.h winsock2.h])

# Check for types
# ===============

# Add OpenGL libraries for gdsoglviewer. This should only be done for 
# that program really!
if test "$enable_ogl" = "yes"; then
	if test "$MINGW32" = "yes"; then
		OPENGL_LIBS="-lopengl32 -lglu32 -lglut32 -lgdi32"
	else
		if test "$SOLARIS" = "yes"; then
			OPENGL_LIBS="-lGLU -lGL -lX11"
		else
			OPENGL_LIBS="-lGLU -lGL"
		fi
	fi
else
	OPENGL_LIBS=""
fi

AC_CHECK_FUNCS([gettimeofday QueryPerformanceCounter])

AC_ARG_ENABLE([profile],
		AC_HELP_STRING([--enable-profile], [enable gprof profiling]),
		[enable_profile=yes],
		[enable_profile=no])

if test "$enable_profile" = yes; then
	CFLAGS="-ggdb -pg -fprofile-arcs -ftest-coverage"
	CXXFLAGS="-ggdb -pg -fprofile-arcs -ftest-coverage"
	CFLAGS="-g -pg -fno-stack-protector-all -fno-stack-protector -nopie"
	CXXFLAGS="-g -pg -fno-stack-protector-all -fno-stack-protector -nopie"
fi

