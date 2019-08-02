# These lists are later turned into target properties on main WW library target

# ---- Google-glog
include("cmake/external/glog.cmake")
list(APPEND WW_INCLUDE_DIRS PUBLIC ${GLOG_INCLUDE_DIRS})
list(APPEND WW_LINKER_LIBS PUBLIC ${GLOG_LIBRARIES})


# ---- Google-gflags
include("cmake/external/gflags.cmake")
list(APPEND WW_INCLUDE_DIRS PUBLIC ${GFLAGS_INCLUDE_DIRS})
list(APPEND WW_LINKER_LIBS PUBLIC ${GFLAGS_LIBRARIES})