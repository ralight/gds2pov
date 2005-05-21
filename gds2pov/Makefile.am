#
# $Id: Makefile.am 223 2005-05-21 11:24:17Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gds2pov

#gds2pov_DATA	

gds2pov_SOURCES =	gds2pov.cpp \
			gds2pov.h \
			gdsobject_pov.cpp \
			gdsobject_pov.h \
			gdsparse_pov.cpp \
			gdsparse_pov.h

gds2pov_LDADD = -lgdsto3d
gds2pov_LDFLAGS = -L../libgdsto3d

