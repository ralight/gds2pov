#
# $Id: Makefile.am 202 2005-03-17 21:00:11Z roger $

SUBDIRS = .

AM_CPPFLAGS = -I$(top_srcdir)/libgdsto3d

bin_PROGRAMS = gds23ds

#gds23ds_DATA	

gds23ds_SOURCES =	gds23ds.cpp \
			gdsobject_3ds.cpp \
			gdsparse_3ds.cpp

gds23ds_LDADD = -lgdsto3d
gds23ds_LDFLAGS = -L../libgdsto3d

