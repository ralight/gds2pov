#
# $Id: Makefile.am 202 2005-03-17 21:00:11Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gds2pov

#gds2pov_DATA	

gds2pov_SOURCES =	gds2pov.cpp \
			gdsobject_pov.cpp \
			gdsparse_pov.cpp

gds2pov_LDADD = -lgdsto3d
gds2pov_LDFLAGS = -L../libgdsto3d

