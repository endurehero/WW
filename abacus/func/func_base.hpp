#ifndef ABACUS_FUNC_FUNC_BASE_HPP
#define ABACUS_FUNC_FUNC_BASE_HPP

#include "abacus/abacus_func_param.hpp"
#include "abacus/core/context.hpp"
#include "abacus/abacus_type.h"
#include "abacus/func/impl/impl_base.hpp"
#include "abacus/core/data_traits.h"


namespace WW{
namespace abacus{
template<typename TargetType, DATA_TYPE DataType,
         template<typename T, DATA_TYPE D, typename P> class Impl,
         template<typename T, typename D> class Param>
class FuncBase{
public:
    typedef Param<TargetType, typename DataTrait<TargetType, DataType>::DType> param_t;
    typedef Impl<TargetType, DataType, param_t> impl_t;
    typedef std::vector<Tensor<TargetType>*> data_vec_t;
    

    FuncBase(){}
    virtual ~FuncBase(){}


    virtual ABACUS_STATUS init(const Context<TargetType>& ctx, const param_t& param) = 0;
    virtual ABACUS_STATUS operator()(const data_vec_t& input, data_vec_t& output) = 0;
    virtual ABACUS_STATUS computeOutputShape(const data_vec_t& input,  const param_t& param, std::vector<Shape>& output) = 0;

protected:
    std::shared_ptr<impl_t> _impl;

    virtual void pickBestImpl(){
        LOG(INFO) << "Not implemented yet!";
    }
    
};

} // namespace abacus
} // namespace WW
#endif