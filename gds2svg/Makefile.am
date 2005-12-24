#
# $Id: Makefile.am 240 2005-12-24 20:11:56Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gds2svg

#gds2svg_DATA	

gds2svg_SOURCES =	gds2svg.cpp \
			gds2svg.h \
			gdsobject_svg.cpp \
			gdsobject_svg.h \
			gdsparse_svg.cpp \
			gdsparse_svg.h

gds2svg_LDADD = -lgdsto3d
gds2svg_LDFLAGS = -L../libgdsto3d

