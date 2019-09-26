cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)
project(WW C CXX)

set(WW_LINKER_LIBS "")
set(WW_INCLUDE_DIRS "")

include(ExternalProject)
include(cmake/utils.cmake)
include(cmake/dependencies.cmake)

#global WW version 0.0.1
set(VERSION_MAJOR "0")
set(VERSION_MINOR "0")
set(VERSION_PATCH "1")
set(VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

#set root dir of modules
set(WW_ROOT ${PROJECT_SOURCE_DIR})
include_directories(${WW_ROOT})

set(WW_ABACUS ${WW_ROOT}/abacus)
set(WW_TEST ${WW_ROOT}/test)
set(WW_UTILS ${WW_ROOT}/utils)

#WW data float precision
wwOption(WW_TYPE_FP64 "define the FP64 for data precision" NO)
wwOption(WW_TYPE_FP32 "define the FP32 for data precision" YES)
wwOption(WW_TYPE_FP16 "define the FP16 for data precision" NO)
wwOption(WW_TYPE_INT8 "define the INT8 for data precision" NO)

#select platform to build
wwOption(USE_GPU "select the build mode for GPU" NO)
wwOption(USE_CPU "select the build mode for CPU" YES)


#platform details
wwOption(USE_CUDA "use nvidia gpu place." YES if USE_GPU)
wwOption(USE_HIP  "use amd gpu and hip" NO if USE_GPU AND NOT USE_CUDA)
wwOption(USE_OCL  "use amd gpu and ocl" NO if USE_GPU AND NOT USE_CUDA)
wwOption(USE_X86  "use intel cpu." YES if USE_CPU)

#compile options for nvidia gpu place
wwOption(USE_CUBLAS "use cublas libs" NO if USE_CUDA)
wwOption(USE_CURAND "use curand libs" NO if USE_CUDA)
wwOption(USE_CUFFT "use cufft libs" NO if USE_CUDA)
wwOption(USE_CUDNN "use cudnn libs" NO if USE_CUDA)

if(NOT DEFINED WW_OUTPUT_PATH)
    set(WW_OUTPUT_PATH "output")
endif()

if(USE_CUDA)
    set(TARGET_GPUARCH 61)
endif()

#build options for cuda
wwOption(BUILD_CUBIN "build with -cubin option in nvcc." NO if USE_CUDA)
wwOption(COMPILE_PTX "return a list of PTX files generated from src." NO if USE_CUDA)

#common build options
wwOption(ENABLE_DEBUG "Enable DEBUG(default) mode." NO)

#build components
wwOption(BUILD_WITH_UNIT_TEST "build WW unit test" NO)

#build target
wwOption(BUILD_SHARED "Build WW shared lib." YES)
wwOption(BUILD_STATIC "Build WW static lib" YES if NOT BUILD_SHARED)

if(ENABLE_DEBUG)
    set(CMAKE_BUILD_TYPE Debug FORCE)
else()
    set(CMAKE_BUILD_TYPE Release FORCE)
endif()

configure_file(
    "${PROJECT_SOURCE_DIR}/cmake/config/ww_config.h.in"
    "${PROJECT_BINARY_DIR}/ww_config.h"
)

include_directories(${PROJECT_BINARY_DIR})

if(USE_CUDA)
    include(cmake/cuda.cmake)
endif()

if(USE_HIP)
    include(cmake/hip.cmake)
endif()
include(cmake/compiler_options.cmake)

# use cblas
include(cmake/altas.cmake)
include_directories(${ATLAS_INCLUDE_DIR})

#-----------------------------------------------
# section: build and install ww
#-----------------------------------------------
add_subdirectory(${WW_ABACUS})

if (BUILD_WITH_UNIT_TEST)
    add_subdirectory(${WW_TEST})
endif()





