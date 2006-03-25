#
# $Id: Makefile.am 223 2005-05-21 11:24:17Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gds2info

#gds2info_DATA	

gds2info_SOURCES =	gds2info.cpp \
			gds2info.h \
			gdsobject_info.cpp \
			gdsobject_info.h \
			gdsparse_info.cpp \
			gdsparse_info.h

gds2info_LDADD = -lgdsto3d
gds2info_LDFLAGS = -L../libgdsto3d

