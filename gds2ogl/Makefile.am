#
# $Id: Makefile.am 202 2005-03-17 21:00:11Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gds2ogl

#gds2ogl_DATA	

gds2ogl_SOURCES =	gds2ogl.cpp \
			gdsobject_ogl.cpp \
			gdsparse_ogl.cpp

gds2ogl_LDADD = -lgdsto3d
gds2ogl_LDFLAGS = -L../libgdsto3d

