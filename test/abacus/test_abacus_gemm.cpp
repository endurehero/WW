
#include "abacus/core/context.hpp"
#include "utils/unit_test/test_suite.h"
#include "test/abacus/test_abacus_func.hpp"
#include "test/abacus/test_abacus_base.hpp"
#include "abacus/abacus_func_param.hpp"
#include "abacus/func/gemm.hpp"

using namespace WW::test;
using namespace WW::abacus;

template<typename DataType, typename TargetType_H, typename TargetType_D>
void gemm_cpu(const std::vector<Tensor<TargetType_H>*>& inputs, std::vector<Tensor<TargetType_H>*>& outputs, const GemmParam<TargetType_D, DataType>& param){
    LOG(INFO) << "++++++++++++++++++++++++++++++++++++";
    LOG(INFO) << "gemm cpu";
    LOG(INFO) << "++++++++++++++++++++++++++++++++++++";
}




TEST(TestAbacusFunc, test_gemm_op){

#ifdef USE_CUDA
    typedef typename DataTrait<NV, DATA_FLOAT>::DType DataType;

    TestAbacusBase<NVHX86, NV, DATA_FLOAT, Gemm, GemmParam> test_base;
    for(int m  : {100, 200, 300}){
        for(int n : {100, 200, 300}){
            for(int k : {100, 200, 300}){
                std::vector<Shape> input_shapes;
                input_shapes.push_back(Shape({1, 1, k, m})); // Matrix A
                input_shapes.push_back(Shape({1, 1, k, m})); // Matrix B
                GemmParam<NV, DataType> param(false, false, m, n, k, m, k, m, 1.0, 0);
                test_base.setParam(param);
                test_base.setInputShape(input_shapes);
                test_base.runTest(gemm_cpu<DataType, NVHX86, NV>);
            }
        }
    }
    
#endif

#ifdef USE_X86
    typedef typename DataTrait<X86, DATA_FLOAT>::DType DataType;

    TestAbacusBase<X86, X86, DATA_FLOAT, Gemm, GemmParam> test_base;
    for(int m  : {100, 200, 300}){
        for(int n : {100, 200, 300}){
            for(int k : {100, 200, 300}){
                std::vector<Shape> input_shapes;
                input_shapes.push_back(Shape({1, 1, k, m})); // Matrix A
                input_shapes.push_back(Shape({1, 1, n, k})); // Matrix B
                GemmParam<X86, DataType> param(false, false, m, n, k, m, k, m, 1.0, 0);
                test_base.setParam(param);
                test_base.setInputShape(input_shapes);
                test_base.runTest(gemm_cpu<DataType, X86, X86>);
            }
        }
    }
#endif
}


int main(int argc, const char** argv){
    
    InitTest();
    RUN_ALL_TESTS(argv[0]);
    return 0;
}