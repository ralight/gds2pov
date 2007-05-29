
ADD_LIBRARY (gdsto3d SHARED config_cfg.cpp gds_globals.cpp gdsobject.cpp
gdsobjects.cpp gdsparse.cpp gdspath.cpp gdspolygon.cpp gdstext.cpp
process_cfg.cpp)

INCLUDE_DIRECTORIES(${gdsto3d_SOURCE_DIR}/libgdsto3d)
LINK_DIRECTORIES(${gdsto3d_BINARY_DIR}/libgdsto3d)
