dnl Process this file with autoconf to produce a configure script.
dnl $Id: configure.ac 221 2005-05-21 10:17:15Z roger $

AC_INIT([libgdsto3d and utilities], [1.0.0], [roger@atchoo.org], [gdsto3d])

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

# Check for header files
# ======================
AC_CHECK_HEADERS([stdarg.h X11/keysym.h X11/Xlib.h GL/gl.h GL/glx.h GL/glu.h sys/time.h time.h windows.h winsock2.h])


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

AC_CHECK_FUNCS([gettimeofday QueryPerformanceCounter])


CFLAGS="${CFLAGS} -ggdb"

AC_SUBST(LDADD)

AC_CONFIG_FILES([Makefile
		libgdsto3d/Makefile
		gds2pov/Makefile
		gds23ds/Makefile
		gds2ogl/Makefile
		gdsoglviewer/Makefile
		])
AC_OUTPUT

