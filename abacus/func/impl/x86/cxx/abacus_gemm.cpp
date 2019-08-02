#include "abacus/func/impl/x86/abacus_gemm.h"

namespace WW{
namespace abacus{



template<DATA_TYPE DataType>
ABACUS_STATUS AbacusGemm<X86, DataType>::forward(const std::vector<Tensor<X86>* >& inputs, std::vector<Tensor<X86>*>& outputs){

    LOG(INFO) << "Run X86 Gemm Kernel.";
}

template class AbacusGemm<X86, DATA_FLOAT>;
} //namespace abacus
} //namespace WW