#!/bin/sh

cd $1
DISTDIR=$2/gdsto3d-$3

mkdir -p ${DISTDIR}

cp	dist.sh \
	config.h.cmake \
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

mkdir -p ${DISTDIR}/libgdsto3d
cp	libgdsto3d/config_cfg.h libgdsto3d/config_cfg.cpp \
	libgdsto3d/gdselements.h \
	libgdsto3d/gdsobject.h libgdsto3d/gdsobject.cpp \
	libgdsto3d/gdsobject_pov.h libgdsto3d/gdsobject_pov.cpp \
	libgdsto3d/gdsobject_svg.h libgdsto3d/gdsobject_svg.cpp \
	libgdsto3d/gdsparse.h libgdsto3d/gdsparse.cpp \
	libgdsto3d/gdsparse_pov.h libgdsto3d/gdsparse_pov.cpp \
	libgdsto3d/gdsparse_svg.h libgdsto3d/gdsparse_svg.cpp \
	libgdsto3d/gdspath.h libgdsto3d/gdspath.cpp \
	libgdsto3d/gdspolygon.h libgdsto3d/gdspolygon.cpp \
	libgdsto3d/gdstext.h libgdsto3d/gdstext.cpp \
	libgdsto3d/gdsto3d.h \
	libgdsto3d/gds_types.h libgdsto3d/gds_types.cpp \
	libgdsto3d/gds_globals.h libgdsto3d/gds_globals.cpp \
	libgdsto3d/process_cfg.h libgdsto3d/process_cfg.cpp \
	libgdsto3d/CMakeLists.txt \
	${DISTDIR}/libgdsto3d/

mkdir -p ${DISTDIR}/gds2pov
cp	gds2pov/gds2pov.cpp \
	gds2pov/CMakeLists.txt \
	${DISTDIR}/gds2pov/

mkdir -p ${DISTDIR}/gds2svg
cp	gds2svg/gds2svg.cpp \
	gds2svg/CMakeLists.txt \
	${DISTDIR}/gds2svg/

mkdir -p ${DISTDIR}/build

tar -jcf ${DISTDIR}.tar.bz2 -C $2 gdsto3d-$3
pushd ${DISTDIR}
zip -9 -r ${DISTDIR}.zip .
popd
#rm -rf ${DISTDIR}

