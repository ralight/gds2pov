SET (GDSTO3D_SRCS config_cfg.cpp gds_globals.cpp gdsobject.cpp gdsobjects.cpp gdsparse.cpp gdspath.cpp gdspolygon.cpp gdstext.cpp process_cfg.cpp)

#IF (WIN32)
#	ADD_LIBRARY (gdsto3d SHARED ${gdsto3d_SRCS} gds_types.cpp)
#ELSE (WIN32)
	ADD_LIBRARY (gdsto3d SHARED ${GDSTO3D_SRCS})
#ENDIF (WIN32)
SET_TARGET_PROPERTIES (gdsto3d PROPERTIES VERSION 0.0.3 SOVERSION 0)

INCLUDE_DIRECTORIES(${gdsto3d_SOURCE_DIR}/libgdsto3d)
LINK_DIRECTORIES(${gdsto3d_BINARY_DIR}/libgdsto3d)

IF (WINSOCK2_LIBRARY)
	#TARGET_LINK_LIBRARIES (gdsto3d ${WINSOCK2_LIBRARY})
	TARGET_LINK_LIBRARIES (gdsto3d c:/unix/mingw/lib/libws2_32.a)
ENDIF (WINSOCK2_LIBRARY)

INSTALL (TARGETS gdsto3d RUNTIME DESTINATION ${BINDEST} LIBRARY DESTINATION ${LIBDEST})
IF (UNIX)
	INSTALL(CODE "EXEC_PROGRAM(/sbin/ldconfig)")
ENDIF (UNIX)
