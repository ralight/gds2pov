#
# $Id: Makefile.am 206 2005-05-09 09:37:23Z roger $

SUBDIRS = .

EXTRA_DIST = 

lib_LTLIBRARIES = libgdsto3d.la

include_HEADERS = gdsto3d.h

libgdsto3d_la_LIBADD = @LDADD@

libgdsto3d_la_LDFLAGS = -version-info 0:0:0

libgdsto3d_la_SOURCES =	config_cfg.cpp \
			gds_globals.cpp \
			gdsobject.cpp \
			gdsobjects.cpp \
			gdsparse.cpp \
			gdspath.cpp \
			gdspolygon.cpp \
			gdstext.cpp \
			process_cfg.cpp

			//gds_types.cpp 

