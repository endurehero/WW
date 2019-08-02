#include "abacus/func/impl/cuda/abacus_gemm.h"

namespace WW{
namespace abacus{

template<typename DataType>
__global__ void gemm(bool transA, bool transB, const int M, const int N, const int K, 
                    const float alpha, DataType* A, const float beta, const DataType* B,
                    float*C){
    

}


template<DATA_TYPE DataType>
ABACUS_STATUS AbacusGemm<NV, DataType>::forward(const std::vector<Tensor<NV>* >& inputs, std::vector<Tensor<NV>*>& outputs){

    LOG(INFO) << "Run Cuda Gemm Kernel.";
}


template class AbacusGemm<NV, DATA_FLOAT>;
} //namespace abacus
} //namespace WW