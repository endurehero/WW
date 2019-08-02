#ifndef ABACUS_FUNC_GEMM_HPP
#define ABACUS_FUNC_GEMM_HPP


#include "abacus/core/shape.hpp"
#include "abacus/abacus_type.h"
#include "abacus/func/impl/impl_base.hpp"
#include "abacus/func/impl/impl_gemm.h"
#include "abacus/func/func_base.hpp"
#include "abacus/abacus_func_param.hpp"

#ifdef USE_CUDA
#include "abacus/func/impl/cuda/abacus_gemm.h"
#endif

#ifdef USE_X86
#include "abacus/func/impl/x86/abacus_gemm.h"
#endif

namespace WW{
namespace abacus{

template<typename TargetType, DATA_TYPE DataType>
class Gemm: public FuncBase<TargetType, DataType, ImplBase, GemmParam>{
public:
    using Base = FuncBase<TargetType, DataType, ImplBase, GemmParam>;

    Gemm() {}
    ~Gemm(){}

    virtual ABACUS_STATUS init(const Context<TargetType>& ctx, const typename Base::param_t& param) override{
        Base::_impl = std::make_shared<AbacusGemm<TargetType, DataType>>();

        if(!Base::_impl){
            LOG(ERROR) << "AbacusGemm instance create failed!";
            return AbacusMemAllocFailed;
        }

        Base::_impl->init(ctx, param);
        
        return AbacusSuccess;
    }
    
    virtual ABACUS_STATUS operator()(const typename Base::data_vec_t& input, typename Base::data_vec_t& output) override{
        if(!Base::_impl){
            LOG(ERROR) << "AbacusGemm instance not initialized!";
            return AbacusNotInitialized;
        }
        
        Base::_impl->forward(input, output);

        return AbacusSuccess;
    }

    virtual ABACUS_STATUS computeOutputShape(const typename Base::data_vec_t& input, const typename Base::param_t& param, std::vector<Shape>& out_shape) override{
        CHECK_EQ(input.size(), 2) << "Invalid input data";
        
        int batch   = input.front()->batch();
        int channel = input.front()->channel();
        
        int height  = param._m;
        int width   = param._n;

        Shape shape({batch, channel, height, width});
        out_shape.emplace_back(shape);
        
        return AbacusSuccess;
    }
};


} // namespace abacus
} //namespace WW
#endif