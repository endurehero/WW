set(WW_ABACUS_BASE_SRC "")

#add include dir
wwFetchIncludeRecursively(${WW_ABACUS})
wwFetchIncludeRecursively(${WW_UTILS})

#add base source files
wwFetchFilesWithSuffix(${WW_ABACUS}/core "cpp" WW_ABACUS_BASE_SRC)
wwFetchFilesWithSuffix(${WW_ABACUS}/func "cpp" WW_ABACUS_BASE_SRC)
wwFetchFilesWithSuffix(${WW_ABACUS}/func/impl "cpp" WW_ABACUS_BASE_SRC)


if(USE_CPU)
    wwFetchFilesWithSuffix(${WW_ABACUS}/core/impl/x86 "cpp" WW_ABACUS_BASE_SRC)
    wwFetchFilesWithSuffix(${WW_ABACUS}/func/impl/x86/cxx "cpp" WW_ABACUS_BASE_SRC)
endif()

if(USE_GPU)
    if(USE_CUDA)
        wwFetchFilesWithSuffix(${WW_ABACUS}/core/impl/cuda "cpp" WW_ABACUS_BASE_SRC)
        wwFetchFilesWithSuffix(${WW_ABACUS}/func/impl/cuda "cpp" WW_ABACUS_BASE_SRC)
    elseif(USE_HIP)
        wwFetchFilesWithSuffix(${WW_ABACUS}/core/impl/hip "cpp" WW_ABACUS_BASE_SRC)
        wwFetchFilesWithSuffix(${WW_ABACUS}/func/impl/hip "cpp" WW_ABACUS_BASE_SRC)
    endif()
endif()

#compile cpp objs
set(WW_ABACUS_LIB "ww_abacus")

if(USE_CUDA)
    set(CUDA_BASE_CODE_ROOT ${WW_ABACUS}/func/impl/cuda/cu)
    add_subdirectory(${CUDA_BASE_CODE_ROOT})
    
    if(BUILD_SHARED)
        CUDA_COMPILE(WW_ABACUS_CUDA_C_SRC_OBJS SHARED ${WW_SABER_CUDA_C_SRC})
    else() # build static
        CUDA_COMPILE(WW_ABACUS_CUDA_C_SRC_OBJS STATIC ${WW_SABER_CUDA_C_SRC})
    endif()

endif()

if(UNIX OR APPLE)
    if(BUILD_SHARED)
        add_library(${WW_ABACUS_LIB} SHARED ${WW_ABACUS_BASE_SRC} ${WW_ABACUS_CUDA_C_SRC_OBJS})
        set_target_properties(${WW_ABACUS_LIB} PROPERTIES VERSION ${VERSION})
        target_include_directories(${WW_ABACUS_LIB} ${WW_INCLUDE_DIRS}
                                                    PUBLIC
                                                    $<BUILD_INTERFACE:${WW_INCLUDE_DIR}>
                                                    $<INSTALL_INTERFACE:include>)
        target_link_libraries(${WW_ABACUS_LIB} ${WW_LINKER_LIBS})
        set_target_properties(${WW_ABACUS_LIB} PROPERTIES LINK_FLAGS "")
        set_target_properties(${WW_ABACUS_LIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
            ${WW_ROOT}/${WW_OUTPUT_PATH}/)
    else() #build static
        add_library(${WW_ABACUS_LIB} STATIC ${WW_ABACUS_BASE_SRC} ${WW_ABACUS_CUDA_C_SRC_OBJS})
        target_include_directories(${WW_ABACUS_LIB} ${WW_INCLUDE_DIRS}
                                                    PUBLIC
                                                    $<BUILD_INTERFACE:${WW_INCLUDE_DIR}>
                                                    $<INSTALL_INTERFACE:include>)
        set_target_properties(${WW_ABACUS_LIB} PROPERTIES LINK_FLAGS "")
        set_target_properties(${WW_ABACUS_LIB} PROPERTIES LIBRARY_OUTPUT_DIRECTORY
            ${WW_ROOT}/${WW_OUTPUT_PATH}/)
    endif()
endif()

set(WW_ABACUS_LIB_TARGET ${WW_ABACUS_LIB} PARENT_SCOPE)

