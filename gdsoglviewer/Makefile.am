#
# $Id: Makefile.am 209 2005-05-09 18:04:20Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gdsoglviewer

#gdsoglviewer_DATA	

gdsoglviewer_SOURCES =	gdsoglviewer.cpp \
			gdsobject_ogl.cpp \
			gdsparse_ogl.cpp

gdsoglviewer_LDADD = -lgdsto3d -lGLU -lGL
gdsoglviewer_LDFLAGS = -L../libgdsto3d

