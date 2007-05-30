#!/bin/sh

cd $1
DISTDIR=$2/gdsto3d-$3

mkdir -p ${DISTDIR}

cp dist.sh config.h.cmake example_config.txt CMakeLists.txt example.cmd example.gds example_process.txt readme.txt todo.txt AUTHORS COPYING changes.txt ${DISTDIR}

mkdir -p ${DISTDIR}/libgdsto3d
cp libgdsto3d/config_cfg.h libgdsto3d/gdspolygon.h libgdsto3d/gdsobjects.h libgdsto3d/gdsobject.h libgdsto3d/process_cfg.cpp libgdsto3d/gds_types.cpp libgdsto3d/gdsobject.cpp libgdsto3d/CMakeLists.txt libgdsto3d/gdsobjects.cpp libgdsto3d/gds_types.h libgdsto3d/gdsparse.cpp libgdsto3d/config_cfg.cpp libgdsto3d/gdsparse.h libgdsto3d/gdselements.h libgdsto3d/gds_globals.h libgdsto3d/gdspath.h libgdsto3d/gds_globals.cpp libgdsto3d/process_cfg.h libgdsto3d/gdstext.h libgdsto3d/gdspolygon.cpp libgdsto3d/gdsto3d.h libgdsto3d/gdspath.cpp libgdsto3d/gdstext.cpp ${DISTDIR}/libgdsto3d/

#mkdir -p ${DISTDIR}/gds2info
#cp gds2info/gds2info gds2info/gdsobject_info.h gds2info/CMakeLists.txt gds2info/gdsobject_info.cpp gds2info/gdsparse_info.h gds2info/gds2info.h gds2info/gdsparse_info.cpp gds2info/gds2info.cpp ${DISTDIR}/gds2info/

#mkdir -p ${DISTDIR}/gds23ds
#cp gds23ds/gdsobject_3ds.cpp gds23ds/CMakeLists.txt gds23ds/gdsparse_3ds.cpp gds23ds/gds23ds.cpp gds23ds/gds23ds.h ${DISTDIR}/gds23ds/

#mkdir -p ${DISTDIR}/gds2ogl
#cp gds2ogl/CMakeLists.txt gds2ogl/gdsparse_ogl.cpp gds2ogl/gds2ogl.cpp gds2ogl/gdsobject_ogl.cpp gds2ogl/gds2ogl.h ${DISTDIR}/gds2ogl/

mkdir -p ${DISTDIR}/gds2pov
cp gds2pov/gdsobject_pov.cpp gds2pov/CMakeLists.txt gds2pov/gdsobject_pov.h gds2pov/gdsparse_pov.cpp gds2pov/gds2pov.cpp gds2pov/gdsparse_pov.h gds2pov/gds2pov.h ${DISTDIR}/gds2pov/

mkdir -p ${DISTDIR}/gds2svg
cp gds2svg/gdsparse_svg.cpp gds2svg/gds2svg.cpp gds2svg/CMakeLists.txt gds2svg/gdsobject_svg.h gds2svg/gdsobject_svg.cpp gds2svg/gdsparse_svg.h gds2svg/gds2svg.h ${DISTDIR}/gds2svg/

mkdir -p ${DISTDIR}/gdsoglviewer
cp gdsoglviewer/CMakeLists.txt gdsoglviewer/gdsparse_ogl.cpp gdsoglviewer/gdsobject_ogl.h gdsoglviewer/gdsoglviewer.cpp gdsoglviewer/gdsoglviewer.h gdsoglviewer/gdsparse_ogl.h gdsoglviewer/gdsobject_ogl.cpp ${DISTDIR}/gdsoglviewer/

tar -jcf ${DISTDIR}.tar.bz2 ${DISTDIR}
pushd ${DISTDIR}
find . -print | zip ${DISTDIR}.zip -@
popd
#rm -rf ${DISTDIR}

