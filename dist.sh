#!/bin/sh

cd $1
DISTDIR=$2/gds2x-$3

mkdir -p ${DISTDIR}

cp	dist.sh \
	example_config.txt \
	CMakeLists.txt \
	example.cmd \
	example.gds \
	example_process.txt \
	readme.txt \
	todo.txt \
	AUTHORS \
	COPYING \
	changes.txt \
	${DISTDIR}

mkdir -p ${DISTDIR}/libgds2x
cp	libgds2x/config_cfg.h libgds2x/config_cfg.cpp \
	libgds2x/gdselements.h \
	libgds2x/gdsobject.h libgds2x/gdsobject.cpp \
	libgds2x/gdsobject_pov.h libgds2x/gdsobject_pov.cpp \
	libgds2x/gdsobject_svg.h libgds2x/gdsobject_svg.cpp \
	libgds2x/gdsparse.h libgds2x/gdsparse.cpp \
	libgds2x/gdsparse_pov.h libgds2x/gdsparse_pov.cpp \
	libgds2x/gdsparse_svg.h libgds2x/gdsparse_svg.cpp \
	libgds2x/gdspath.h libgds2x/gdspath.cpp \
	libgds2x/gdspolygon.h libgds2x/gdspolygon.cpp \
	libgds2x/gdstext.h libgds2x/gdstext.cpp \
	libgds2x/gds2x.h \
	libgds2x/gds_types.h libgds2x/gds_types.cpp \
	libgds2x/gds_globals.h libgds2x/gds_globals.cpp \
	libgds2x/process_cfg.h libgds2x/process_cfg.cpp \
	libgds2x/CMakeLists.txt \
	${DISTDIR}/libgds2x/

mkdir -p ${DISTDIR}/gds2pov
cp	gds2pov/gds2pov.cpp \
	gds2pov/CMakeLists.txt \
	${DISTDIR}/gds2pov/

mkdir -p ${DISTDIR}/gds2svg
cp	gds2svg/gds2svg.cpp \
	gds2svg/CMakeLists.txt \
	${DISTDIR}/gds2svg/

mkdir -p ${DISTDIR}/build

tar -jcf ${DISTDIR}.tar.bz2 -C $2 gds2x-$3
pushd ${DISTDIR}
zip -9 -r ${DISTDIR}.zip .
popd
#rm -rf ${DISTDIR}

