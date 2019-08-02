macro(wwFindCuda)
    if(BUILD_CUBIN)
        set(CUDA_BUILD_CUBIN ON)
    endif()

    find_package(CUDA 7.5 REQUIRED)
    set(CUDA_HOST_COMPILER ${CMAKE_C_COMPILER})
    if(BUILD_SHARED)
        if(CUDA_FOUND)
            include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
            list(APPEND WW_LINKER_LIBS ${CUDA_CUDART_LIBRARY})

            if(USE_CUBLAS)
                list(APPEND WW_LINKER_LIBS ${CUDA_CUBLAS_LIBRARIES})
            endif()

            if(USE_CURAND)
                list(APPEND WW_LINKER_LIBS ${CUDA_curand_LIBRARY})
            endif()
        else()
            message(FATAL_ERROR "Cuda shared lib could not found!")
        endif()
    else() # BUILD_STATIC
        find_path(CUDA_INCLUDE_DIRS cuda.h PATHS /usr/local/cuda/include_directories
                                                 /usr/include)
        if(CUDA_INCLUDE_DIRS)
            include_directories(SYSTEM ${CUDA_INCLUDE_DIRS})
            find_library(CUDA_LIBRARY NAMES libcudart_static.a
                                      PATHS ${CUDA_INCLUDE_DIRS}/../lib64
                                      DOC "library path for cuda")
            
            if(CUDA_LIBRARY)
                list(APPEND WW_LINKER_LIBS ${CUDA_INCLUDE_DIRS}/../lib64/libcudart_static.a)
                if(USE_CUBLAS)
                    list(APPEND WW_LINKER_LIBS ${CUDA_INCLUDE_DIRS}/../lib64/libcublas_static.a)
                    list(APPEND WW_LINKER_LIBS ${CUDA_INCLUDE_DIRS}/../lib64/libcublas_device.a)
                endif()
                if(USE_CURAND)
                    list(APPEND WW_LINKER_LIBS ${CUDA_INCLUDE_DIRS}/../lib64/libcurand_static.a)
                endif()
                list(APPEND WW_LINKER_LIBS ${CUDA_INCLUDE_DIRS}/../lib64/libculibos.a)
            endif()
        else()
            message(FATAL_ERROR "cuda static lib could not found !")
        endif()
    endif()

    # build cuda
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};--generate-code arch=compute_${TARGET_GPUARCH}, code=sm_${TARGET_GPUARCH})
endmarco()