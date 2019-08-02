set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=address")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=sequence-point")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-variable") # no unused-variable
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-declarations")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Winit-self")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wpointer-arith")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-promo")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-show-option")

if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gdwarf-2") # for old version gcc and gdb. see: http://stackoverflow.com/a/15051109/673852
else() # Release
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNDEBUG")
endif()


if(USE_CUDA)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-Xcompiler -fPIC)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-std=c++11)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};--default-stream per-thread)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-Wno-deprecated-gpu-targets)
    if(CMAKE_BUILD_TYPE MATCHS Debug)
        set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-G)
        set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-g)
    else() # Release
        set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-O3)
    endif()
endif()