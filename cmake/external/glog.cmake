#glog depends on gflags
include("cmake/external/gflags.cmake")

if(NOT __GLOG_INCLUDED)
    set(__GLOG_INCLUDED TRUE)
    
    #try the system-wide glog first
    find_package(Glog)
    
    if(GLOG_FOUND)
        set(GLOG_EXTERNAL FALSE)
    else()
        # fetch and build glog from github
        
        # build directory
        set(glog_PREFIX ${CMAKE_BINARY_DIR}/external/glog_prefix)
        # install directory
        set(glog_INSTALL ${CMAKE_BINARY_DIR}/external/glog-install)

        if(UNIX)
            set(GLOG_EXTRA_COMPILER_FLAGS "-fPIC")
        endif()

        set(GLOG_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GLOG_EXTRA_COMPILER_FLAGS})
        set(GLOG_C_FLAGS ${CMAKE_C_FLAGS} ${GLOG_EXTRA_COMPILER_FLAGS})
    
        if(GFLAGS_EXTERNAL)
            set(GLOG_DEPENDS gflags)
        endif()

        ExternalProject_Add(glog
            DEPENDS ${glog_DEPENDS}
            PREFIX ${glog_PREFIX}
            GIT_REPOSITORY "https://github.com/google/glog"
            GIT_TAG "v0.3.4"
            UPDATE_COMMAND ""
            INSTALL_DIR ${gflags_INSTALL}
            PATCH_COMMAND autoreconf -i ${glog_PREFIX}/src/glog
            CONFIGURE_COMMAND  env "CFLAGS=${GLOG_C_FLAGS}" "CXXFLAGS=${GLOG_CXX_FLAGS}" ${glog_PREFIX}/src/glog/configure --prefix=${glog_INSTALL} --enable-shared=no --enable-static=yes --with-gflags=${GFLAGS_LIBRARY_DIRS}/..
            LOG_DOWNLOAD 1
            LOG_CONFIGURE 1
            LOG_INSTALL 1
        )

        set(GLOG_FOUND TRUE)
        set(GLOG_INCLUDE_DIRS ${glog_INSTALL/include})
        set(GLOG_LIBRARIES ${GFLAGS_LIBRARIES} ${glog_INSTALL}/lib/libglog.a)
        set(GLOG_LIBRARY_DIRS ${glog_INSTALL}/lib)
        set(GLOG_EXTERNAL TRUE)

        list(APPEND external_project_dependencies __glog)
    endif()
endif()