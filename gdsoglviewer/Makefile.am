#
# $Id: Makefile.am 225 2005-05-21 19:53:25Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gdsoglviewer

#gdsoglviewer_DATA	

gdsoglviewer_SOURCES =	gdsoglviewer.cpp \
			gdsoglviewer.h \
			gdsobject_ogl.cpp \
			gdsobject_ogl.h \
			gdsparse_ogl.cpp \
			gdsparse_ogl.h

#gdsoglviewer_LDADD = -lgdsto3d -lGLU -lGL
gdsoglviewer_LDADD = -lgdsto3d $(OPENGL_LIBS)
gdsoglviewer_LDFLAGS = -L../libgdsto3d

