#
# $Id: Makefile.am 202 2005-03-17 21:00:11Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gdsoglviewer

#gdsoglviewer_DATA	

gdsoglviewer_SOURCES =	gdsoglviewer.cpp \
			gdsobject_pov.cpp \
			gdsparse_pov.cpp

gdsoglviewer_LDADD = -lgdsto3d
gdsoglviewer_LDFLAGS = -L../libgdsto3d

