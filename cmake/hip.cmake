set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE )
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${WW_ROOT}/cmake/external)

find_package( HIP REQUIRED )
set (CMAKE_CXX_COMPILER ${HIPCC})
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR} )
list(APPEND WW_INCLUDE_DIRS PUBLIC ${HIP_INCLUDE_DIRS} ${HCC_INCLUDE_DIRS})
list(APPEND WW_LINKER_LIBS PUBLIC ${HSA_LIBRARIES})