#ifndef ABACUS_FUNC_IMPL_IMPL_BASE_HPP
#define ABACUS_FUNC_IMPL_IMPL_BASE_HPP

#include <memory>
#include <vector>

#include "abacus/core/context.hpp"
#include "abacus/core/tensor.hpp"

namespace WW{
namespace abacus{

template <typename TargetType, DATA_TYPE DataType, 
          typename Param>
class ImplBase{
public:
    ImplBase(){}
    virtual ~ImplBase(){}

    virtual ABACUS_STATUS init(const Context<TargetType>& ctx, const Param& param){
        return AbacusUnImplError;
    }

    virtual ABACUS_STATUS create(){
        return AbacusUnImplError;
    }
    
    virtual ABACUS_STATUS forward(const std::vector<Tensor<TargetType>* >& inputs, std::vector<Tensor<TargetType> *>& outputs){
        return AbacusUnImplError;
    }
    
protected:
    std::shared_ptr<Param> _param;
    std::shared_ptr<Context<TargetType>> _ctx;

};
} // namepsace abacus
} //namespace WW
#endif