#ifndef ABACUS_CORE_DATA_TRAITS_H
#define ABACUS_CORE_DATA_TRAITS_H

#include "../abacus_type.h"

namespace WW{
namespace abacus{


inline size_t typeByteNum(DATA_TYPE type){
    switch(type){
        case DATA_VOID:
            return 0;
        case DATA_BOOL:
        case DATA_INT8:
        case DATA_UINT8:
            return 1;
        case DATA_HALF:
        case DATA_INT16:
        case DATA_UINT16:
            return 2;
        case DATA_INT32:
        case DATA_UINT32:
        case DATA_FLOAT:
            return 4;
        case DATA_INT64:
        case DATA_UINT64:
        case DATA_DOUBLE:
            return 8;
        default:
            return 4;
    }
}

template<typename TargetType, DATA_TYPE type>
struct DataTrait{
    typedef __INVALID_TYPE DType;
    typedef __INVALID_TYPE PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_VOID>{
    typedef void DType;
    typedef void* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_BOOL>{
    typedef bool DType;
    typedef bool* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_INT8>{
    typedef char DType;
    typedef char* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_UINT8>{
    typedef unsigned char DType;
    typedef unsigned char* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_HALF>{
    typedef short DType;
    typedef short* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_INT16>{
    typedef short DType;
    typedef short* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_UINT16>{
    typedef unsigned short DType;
    typedef unsigned short* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_INT32>{
    typedef int DType;
    typedef int* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_UINT32>{
    typedef unsigned int DType;
    typedef unsigned int* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_FLOAT>{
    typedef float DType;
    typedef float* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_INT64>{
    typedef long DType;
    typedef long* PtrDType;
};


template<typename TargetType>
struct DataTrait<TargetType, DATA_UINT64>{
    typedef unsigned long DType;
    typedef unsigned long* PtrDType;
};

template<typename TargetType>
struct DataTrait<TargetType, DATA_DOUBLE>{
    typedef double DType;
    typedef double* PtrDType;
};

}// namespace abacus
}// namespace WW
#endif