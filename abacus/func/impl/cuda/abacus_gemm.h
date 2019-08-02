#ifndef ABACUS_FUNC_IMPL_CUDA_ABACUS_GEMM_H
#define ABACUS_FUNC_IMPL_CUDA_ABACUS_GEMM_H

#include "abacus/func/impl/impl_gemm.h"
#include "abacus/abacus_type.h"
#include "abacus/core/data_traits.h"
#include "abacus/abacus_func_param.hpp"

namespace WW{
namespace abacus{

template<DATA_TYPE DataType>
class AbacusGemm<NV, DataType> : public ImplBase<NV, DataType, GemmParam>{
public:
    using ImplBase<NV, DataType, GemmParam>;
    typedef typename DataTrait<NV, DataType>::DType OpDType;
    
    AbacusGemm(){}
    ~AbacusGemm(){}

    ABACUS_STATUS init(Context<NV>& ctx, GemmParam<NV> &param){
        _ctx = std::make_shared<Context<NV>>(ctx);
        _param = std::make_shared<GemmParam<NV>>(param);

        create();
        return AbacusSuccess;
    }

    ABACUS_STATUS create(){}
    ABACUS_STATUS forward(const std::vector<Tensor<NV>* >& inputs, std::vector<Tensor<NV>*>& outputs);
};

} // namespace abacus
} //namespace WW
#endif