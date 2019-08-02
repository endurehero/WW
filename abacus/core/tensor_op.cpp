#include <random>
#include <limits.h>
#include <stdio.h>
#include <cmath>

#include "abacus/core/tensor_op.h"
#include "abacus/core/target_traits.h"
#include "abacus/abacus_type.h"

namespace WW{
namespace abacus{


using namespace std;

const float eps = 1e-06;
const int print_line_width = 8;

using std::default_random_engine;
using std::uniform_int_distribution;
using std::uniform_real_distribution;

template <typename DataType>
void fillTensorHostConstImpl(DataType* ptr, DataType value, long long size){
    for(long long i = 0; i < size; ++i){
        ptr[i] = value;
    }
}

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDataHostConst(Tensor<TargetType>& tensor, float val, typename Tensor<TargetType>::API::stream_t stream){
    bool is_same = std::is_same<typename TargetTraits<TargetType>::TargetCategory, __HOST_TARGET>::value;
    CHECK_EQ(is_same, true) \
        << "this function handles only the host sieds.";
    
    long long data_size = tensor.size();
    void* ptr = tensor.dataMutable();
    
    typedef typename DataTrait<TargetType, DataT>::DType DataType;
    typedef typename DataTrait<TargetType, DataT>::PtrDType PtrDataType;
    
    is_same = std::is_same<void, DataType>::value || std::is_same<__INVALID_TYPE, DataType>::value;
    CHECK_EQ(is_same, false) \
        << "invalid tensor type!";
    
    fillTensorHostConstImpl(static_cast<PtrDataType>(ptr), static_cast<DataType>(val), data_size);
    
}

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDiffHostConst(Tensor<TargetType>& tensor, float val, typename Tensor<TargetType>::API::stream_t stream){
    bool is_same = std::is_same<typename TargetTraits<TargetType>::TargetCategory, __HOST_TARGET>::value;
    CHECK_EQ(is_same, true) \
        << "this function handles only the host sieds.";
    
    long long data_size = tensor.size();
    void* ptr = tensor.diffMutable();

    typedef typename DataTrait<TargetType, DataT>::DType DataType;
    typedef typename DataTrait<TargetType, DataT>::PtrDType PtrDataType;
    
    is_same = std::is_same<void, DataType>::value || std::is_same<__INVALID_TYPE, DataType>::value;
    CHECK_EQ(is_same, false) \
        << "invalid tensor type!";
    
    fillTensorHostConstImpl(static_cast<PtrDataType>(ptr), static_cast<DataType>(val), data_size);
}


template <typename DataType>
void fillTensorHostRandImpl(DataType* ptr, long long size) {
    default_random_engine e;
    uniform_real_distribution<DataType> u(-1, 1);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }

}

template <>
void fillTensorHostRandImpl<char>(char* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<char> u(SCHAR_MIN, SCHAR_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<unsigned char>(unsigned char* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<unsigned char> u(0, UCHAR_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<short>(short* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<short> u(SHRT_MIN, SHRT_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<unsigned short>(unsigned short* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<unsigned short> u(0, USHRT_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<int>(int* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<int> u(INT_MIN, INT_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<unsigned int>(unsigned int* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<unsigned int> u(0, UINT_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<long long>(long long* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<long long> u(LLONG_MIN, LLONG_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template <>
void fillTensorHostRandImpl<unsigned long long>(unsigned long long* ptr, long long size){
    default_random_engine e;
    uniform_int_distribution<unsigned long long> u(0, ULLONG_MAX);
    
    for(long long i = 0; i < size; ++i){
        ptr[i] = u(e);
    }
}

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDataHostRand(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream){
    long long data_size = tensor.size();
    void* ptr = tensor.dataMutable();

    typedef typename DataTrait<TargetType, DataT>::DType DataType;
    typedef typename DataTrait<TargetType, DataT>::PtrDType PtrDataType;
    
    fillTensorHostRandImpl(static_cast<PtrDataType>(ptr), data_size);
    
}

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDiffHostRand(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream){
    long long data_size = tensor.size();
    void* ptr = tensor.diffMutable();

    typedef typename DataTrait<TargetType, DataT>::DType DataType;
    typedef typename DataTrait<TargetType, DataT>::PtrDType PtrDataType;
    
    fillTensorHostRandImpl(static_cast<PtrDataType>(ptr), data_size);
}

template<typename TargetType>
void fillTensorDataHostRand(Tensor<TargetType>& tensor, float val_start, float val_end, typename Tensor<TargetType>::API::stream_t stream){
    LOG(ERROR) << "Unimplemented yet!";
}

template<typename TargetType>
void fillTensorDiffHostRand(Tensor<TargetType>& tensor, float val_start, float val_end, typename Tensor<TargetType>::API::stream_t stream){
    LOG(ERROR) << "Unimplemented yet!";
}


template<typename DataType>
void printTenosrHostDataImpl(const DataType* ptr, long long size, int line_width){
    for(long long i = 0; i < size; ++i){
        printf("%.6f ", static_cast<float>(ptr[i]));
        if(0 == (i + 1) % line_width) printf("\n");
    }

    printf("\n");
}

template<typename TargetType, DATA_TYPE DataT>
void printTensorHostData(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream){
    LOG(INFO) << "host tensor data: " << tensor.size();
    void* ptr = tensor.dataMutable();
    long long data_size = tensor.size();

    typedef typename DataTrait<TargetType, DataT>::DType DataType;
    typedef typename DataTrait<TargetType, DataT>::PtrDType PtrDataType;
    printTenosrHostDataImpl(static_cast<PtrDataType>(ptr), data_size, print_line_width);
    
}

template<typename TargetType, DATA_TYPE DataT>
void printTensorHostDiff(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream){
    LOG(INFO) << "host tensor diff: " << tensor.size();
    void* ptr = tensor.diffMutable();
    long long data_size = tensor.size();

    typedef typename DataTrait<TargetType, DataT>::DType DataType;
    typedef typename DataTrait<TargetType, DataT>::PtrDType PtrDataType;
    printTenosrHostDataImpl(static_cast<PtrDataType>(ptr), data_size, print_line_width);
}



template<typename DataType>
void tensorCmpHost(const DataType* src1, const DataType* src2, size_t size, double& max_ratio, double& max_diff){
    if(0 == size || nullptr == src1 || nullptr == src2){
        LOG(ERROR) << "Invalid input params: src1 = " << src1 << ", src2 = " << src2 << ", size = " << size;
        return;
    }
    
    max_diff = fabs(src1[0] - src2[0]);
    max_ratio = fabs(2.0 * max_diff/ (src1[0] + src2[0] + eps));

    for(int i = 1; i < size; ++i){
        double diff = fabs(src1[i] - src2[i]);
        
        if(max_diff < diff){
            max_diff = diff;
            max_ratio = fabs(2.0 * max_diff/ (src1[i] + src2[i] + eps));    
        }
    }
}

template void fillTensorDataHostConst<X86, DATA_FLOAT>(Tensor<X86>& tensor, float val, typename Tensor<X86>::API::stream_t stream);
template void fillTensorDataHostConst<NVHX86, DATA_FLOAT>(Tensor<NVHX86>& tensor, float val, typename Tensor<NVHX86>::API::stream_t stream);
template void fillTensorDataHostConst<AMDOCLHX86, DATA_FLOAT>(Tensor<AMDOCLHX86>& tensor, float val, typename Tensor<AMDOCLHX86>::API::stream_t stream);
template void fillTensorDataHostConst<AMDHIPHX86, DATA_FLOAT>(Tensor<AMDHIPHX86>& tensor, float val, typename Tensor<AMDHIPHX86>::API::stream_t stream);

template void fillTensorDiffHostConst<X86, DATA_FLOAT>(Tensor<X86>& tensor, float val, typename Tensor<X86>::API::stream_t stream);
template void fillTensorDiffHostConst<NVHX86, DATA_FLOAT>(Tensor<NVHX86>& tensor, float val, typename Tensor<NVHX86>::API::stream_t stream);
template void fillTensorDiffHostConst<AMDOCLHX86, DATA_FLOAT>(Tensor<AMDOCLHX86>& tensor, float val, typename Tensor<AMDOCLHX86>::API::stream_t stream);
template void fillTensorDiffHostConst<AMDHIPHX86, DATA_FLOAT>(Tensor<AMDHIPHX86>& tensor, float val, typename Tensor<AMDHIPHX86>::API::stream_t stream);

template void fillTensorDataHostRand<X86, DATA_FLOAT>(Tensor<X86>& tensor, typename Tensor<X86>::API::stream_t stream);
template void fillTensorDataHostRand<NVHX86, DATA_FLOAT>(Tensor<NVHX86>& tensor, typename Tensor<NVHX86>::API::stream_t stream);
template void fillTensorDataHostRand<AMDOCLHX86, DATA_FLOAT>(Tensor<AMDOCLHX86>& tensor, typename Tensor<AMDOCLHX86>::API::stream_t stream);
template void fillTensorDataHostRand<AMDHIPHX86, DATA_FLOAT>(Tensor<AMDHIPHX86>& tensor, typename Tensor<AMDHIPHX86>::API::stream_t stream);

template void fillTensorDiffHostRand<X86, DATA_FLOAT>(Tensor<X86>& tensor, typename Tensor<X86>::API::stream_t stream);
template void fillTensorDiffHostRand<NVHX86, DATA_FLOAT>(Tensor<NVHX86>& tensor, typename Tensor<NVHX86>::API::stream_t stream);
template void fillTensorDiffHostRand<AMDOCLHX86, DATA_FLOAT>(Tensor<AMDOCLHX86>& tensor, typename Tensor<AMDOCLHX86>::API::stream_t stream);
template void fillTensorDiffHostRand<AMDHIPHX86, DATA_FLOAT>(Tensor<AMDHIPHX86>& tensor, typename Tensor<AMDHIPHX86>::API::stream_t stream);



template void tensorCmpHost<float>(const float* src1, const float* src2, size_t size, double& max_ratio, double& max_diff);


template void printTensorHostData<X86, DATA_FLOAT>(Tensor<X86>& tensor, typename Tensor<X86>::API::stream_t stream);
template void printTensorHostData<NV, DATA_FLOAT>(Tensor<NV>& tensor, typename Tensor<NV>::API::stream_t stream);
template void printTensorHostData<AMD_HIP, DATA_FLOAT>(Tensor<AMD_HIP>& tensor, typename Tensor<AMD_HIP>::API::stream_t stream);
template void printTensorHostData<AMD_OCL, DATA_FLOAT>(Tensor<AMD_OCL>& tensor, typename Tensor<AMD_OCL>::API::stream_t stream);
template void printTensorHostData<NVHX86, DATA_FLOAT>(Tensor<NVHX86>& tensor, typename Tensor<NVHX86>::API::stream_t stream);
template void printTensorHostData<AMDOCLHX86, DATA_FLOAT>(Tensor<AMDOCLHX86>& tensor, typename Tensor<AMDOCLHX86>::API::stream_t stream);
template void printTensorHostData<AMDHIPHX86, DATA_FLOAT>(Tensor<AMDHIPHX86>& tensor, typename Tensor<AMDHIPHX86>::API::stream_t stream);

template void printTensorHostDiff<X86, DATA_FLOAT>(Tensor<X86>& tensor, typename Tensor<X86>::API::stream_t stream);
template void printTensorHostDiff<NV, DATA_FLOAT>(Tensor<NV>& tensor, typename Tensor<NV>::API::stream_t stream);
template void printTensorHostDiff<AMD_HIP, DATA_FLOAT>(Tensor<AMD_HIP>& tensor, typename Tensor<AMD_HIP>::API::stream_t stream);
template void printTensorHostDiff<AMD_OCL, DATA_FLOAT>(Tensor<AMD_OCL>& tensor, typename Tensor<AMD_OCL>::API::stream_t stream);
template void printTensorHostDiff<NVHX86, DATA_FLOAT>(Tensor<NVHX86>& tensor, typename Tensor<NVHX86>::API::stream_t stream);
template void printTensorHostDiff<AMDOCLHX86, DATA_FLOAT>(Tensor<AMDOCLHX86>& tensor, typename Tensor<AMDOCLHX86>::API::stream_t stream);
template void printTensorHostDiff<AMDHIPHX86, DATA_FLOAT>(Tensor<AMDHIPHX86>& tensor, typename Tensor<AMDHIPHX86>::API::stream_t stream);
} // namespace abacus 
} // namespace WW