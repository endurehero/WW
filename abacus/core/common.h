#ifndef ABACUS_CORE_COMMON_H
#define ABACUS_CORE_COMMON_H

#include "utils/log/logger.h"
#include "abacus/abacus_type.h"
#include "ww_config.h"

#ifdef USE_CUDA
#include<cuda_runtime.h>

#define CUDA_CHECK(condition)\
    do{\
        cudaError_t err = condition;\
        CHECK_EQ(err, cudaSuccess) << " " << cudaGetErrorString(err);\
    } while(0)
#endif // USE_CUDA

#ifdef USE_HIP
#include <hip/hip_runtime.h>

#define HIP_CHECK(condition)\
    do{\
        hipError_t err = condition;\
        CHECK_EQ(err, hipSuccess) << " " << hipGetErrorString(err);\
    } while(0)
#endif // USE_HIP

namespace WW{
namespace abacus{

static inline const char* getErrorString(ABACUS_STATUS err_code){
    switch(err_code){
        case AbacusSuccess:
            return "WW_ABACUS_STATUS_SUCCESS";
        case AbacusNotInitialized:
            return "WW_ABACUS_NOT_INITIALIZED";
        case AbacusInvalidValue:
            return "WW_ABACUS_INVALID_VALUE";
        case AbacusMemAllocFailed:
            return "WW_ABACUS_MEM_ALLOC_FAILED";
        case AbacusUnknowError:
            return "WW_ABACUS_UNKNOW_ERROR";
        case AbacusOutofAuthority:
            return "WW_ABACUS_OUT_AUTHORITY";
        case AbacusOutofMem:
            return "WW_ABACUS_OUT_OF_MEM";
        case AbacusUnImplError:
            return "WW_ABACUS_UNIMPL_ERROR";
        case AbacusWrongDevice:
            return "WW_ABACUS_WRONG_DEVICE";
    }

    return "WW_ABACUS_UNKNOW_ERROR";
}

template <bool B, typename ThenType, typename ElseType>
struct IFELSE{
    typedef ThenType TYPE; // true
};
template <typename ThenType, typename ElseType>
struct IFELSE<false, ThenType, ElseType>{
    typedef ElseType TYPE; // false
};

#define ABACUS_CHECK(condition) \
    do{\
        ABACUS_STATUS err = condition;\
        CHECK_EQ(err, AbacusSuccess) << " " << getErrorString(err);\
    } while(0)




}//namespace abacus    
}//namespace WW
#endif