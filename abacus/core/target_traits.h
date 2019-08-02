#ifndef ABACUS_CORE_TARGET_TRAITS_H
#define ABACUS_CORE_TARGET_TRAITS_H

#include "../abacus_type.h"

namespace WW{
namespace abacus{

template<class __TargetType>
struct TargetTraits{
    typedef __INVALID_TYPE TargetCategory;
    typedef __INVALID_TYPE TargetType;
};

template<>
struct TargetTraits<NVHX86>{
    typedef __HOST_TARGET TargetCategory;
    typedef __X86_DEVICE TargetType;
};

template<>
struct TargetTraits<NV>{
    typedef __DEVICE_TARGET TargetCategory;
    typedef __CUDA_DEVICE TargetType;
};

template<>
struct TargetTraits<X86>{
    typedef __HOST_TARGET TargetCategory;
    typedef __X86_DEVICE TargetType;
};

template<>
struct TargetTraits<AMD_HIP>{
    typedef __DEVICE_TARGET TargetCategory;
    typedef __HIP_DEVICE TargetType;
};

template<>
struct TargetTraits<AMDHIPHX86>{
    typedef __HOST_TARGET TargetCategory;
    typedef __X86_DEVICE TargetType;
};

template<>
struct TargetTraits<AMD_OCL>{
    typedef __DEVICE_TARGET TargetCategory;
    typedef __OCL_DEVICE TargetType;
};

template<>
struct TargetTraits<AMDOCLHX86>{
    typedef __HOST_TARGET TargetCategory;
    typedef __X86_DEVICE TargetType;
};

}//namespace abacus
}//namespace WW
#endif