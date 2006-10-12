dnl Process this file with autoconf to produce a configure script.
dnl $Id: configure.ac 271 2006-10-12 22:18:19Z roger $

AC_INIT([libgdsto3d and utilities], [20061012], [roger@atchoo.org], [gdsto3d])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE
AM_MAINTAINER_MODE

# Checks for programs
# ===================

AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_LIBTOOL_WIN32_DLL
#AC_OUTPUT(Makefile)

AM_PROG_LIBTOOL
AC_PROG_RANLIB

# Special case for compiling on windows
# =====================================

case $host_os in
  *mingw32* ) MINGW32=yes;;
          * ) MINGW32=no;;
esac

case $host_os in
  *solaris* ) SOLARIS=yes;;
  	      * ) SOLARIS=no;;
esac

# Check for header files
# ======================
AC_CHECK_HEADERS([stdarg.h X11/keysym.h X11/Xlib.h GL/gl.h GL/glx.h GL/glu.h stdint.h sys/time.h time.h windows.h winsock2.h])

AC_CHECK_HEADERS([GL/gl.h GL/glx.h GL/glu.h], [enable_ogl=yes], [enable_ogl=no])
# Check for types
# ===============

AC_CHECK_TYPE([u_int16_t], [],
[
	AC_CHECK_TYPE([uint16_t],
	[
		AC_DEFINE(u_int16_t, [uint16_t], [If system does not define u_int16_t, define a usable substitute])
	],
	[
		AC_MSG_WARN([system has no u_int16_t or uint16_t, default to unsigned short int])
		AC_DEFINE(u_int16_t, [unsigned short int], [If system does not define u_int16_t, define a usable substitute.])
	])
])

AC_CHECK_FUNC([sinf], [],
[
	AC_DEFINE(sinf, [sin], [Use sin instead of sinf if sinf is not defined])
])
AC_CHECK_FUNC([asinf], [],
[
	AC_DEFINE(asinf, [asin], [Use asin instead of asinf if asinf is not defined])
])

# Add DLL build support
# =====================

if test "$MINGW32" = "yes"; then
	LDFLAGS="$LDFLAGS -no-undefined"
fi


# Check for library functions
# ===========================
# On some systems additional libraries may be needed. Add them if they
# are they.
if test "$MINGW32" = "yes"; then
	LDADD="$LDADD -lws2_32"
fi

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
AC_SUBST(OPENGL_LIBS)

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

#CFLAGS="${CFLAGS} -ggdb"

if test "$enable_ogl" = "yes"; then
	SUBDIRS="libgdsto3d gds2pov gdsoglviewer gds2svg"
else
	SUBDIRS="libgdsto3d gds2pov gds2svg"
fi

AC_SUBST(SUBDIRS)
AC_SUBST(LDADD)

AC_CONFIG_FILES([Makefile
		libgdsto3d/Makefile
		gds2pov/Makefile
		gdsoglviewer/Makefile
		gds2svg/Makefile
		])
#		gds2info/Makefile
		#gds2ogl/Makefile
		#gds23ds/Makefile
AC_OUTPUT

