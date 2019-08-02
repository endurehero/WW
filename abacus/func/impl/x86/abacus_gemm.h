#ifndef ABACUS_FUNC_IMPL_X86_ABACUS_GEMM_H
#define ABACUS_FUNC_IMPL_X86_ABACUS_GEMM_H

#include "abacus/func/impl/impl_gemm.h"
#include "abacus/abacus_type.h"
#include "abacus/core/data_traits.h"
#include "abacus/abacus_func_param.hpp"

namespace WW{
namespace abacus{

template<DATA_TYPE DataType>
class AbacusGemm<X86, DataType> : public ImplBase<X86, DataType, GemmParam<X86, typename DataTrait<X86, DataType>::DType>>{
public:
    typedef typename DataTrait<X86, DataType>::DType OpDType;
       using Base = ImplBase<X86, DataType, GemmParam<X86, OpDType>>;
    
    AbacusGemm(){}
    ~AbacusGemm(){}

    ABACUS_STATUS init(Context<X86>& ctx, GemmParam<X86, OpDType> &param){
        Base::_ctx = std::make_shared<Context<X86>>(ctx);
        Base::_param = std::make_shared<GemmParam<X86, OpDType>>(param);

        create();
        return AbacusSuccess;
    }

    ABACUS_STATUS create(){ return AbacusSuccess; }
    ABACUS_STATUS forward(const std::vector<Tensor<X86>* >& inputs, std::vector<Tensor<X86>*>& outputs);
};

} // namespace abacus
} //namespace WW
#endif