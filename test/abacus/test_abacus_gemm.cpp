
#include "abacus/core/context.hpp"
#include "utils/unit_test/test_suite.h"
#include "test/abacus/test_abacus_func.hpp"
#include "test/abacus/test_abacus_base.hpp"
#include "abacus/abacus_func_param.hpp"
#include "abacus/func/gemm.hpp"

#include <cblas.h>

using namespace WW::test;
using namespace WW::abacus;

template<typename DataType, typename TargetType_H, typename TargetType_D>
void gemm_cpu(const std::vector<Tensor<TargetType_H>*>& inputs, std::vector<Tensor<TargetType_H>*>& outputs, const GemmParam<TargetType_D, DataType>& param){
    LOG(INFO) << "++++++++++++++++++++++++++++++++++++";
    LOG(INFO) << "gemm cpu start...";
    

    CBLAS_TRANSPOSE trans_x = param._transpose_x ? CblasTrans : CblasNoTrans;
    CBLAS_TRANSPOSE trans_y = param._transpose_y ? CblasTrans : CblasNoTrans;

    cblas_sgemm(CblasColMajor, 
                trans_x, trans_y, 
                param._m, param._n, param._k, 
                param._alpha, static_cast<float*>(inputs[0]->dataMutable()), param._lda, 
                static_cast<float*>(inputs[1]->dataMutable()), param._ldb, param._beta, 
                static_cast<float*>(outputs[0]->dataMutable()), param._ldc);
    
    LOG(INFO) << "gemm cpu end.";
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
                GemmParam<NV, DataType> param(false, false, m, n, k, m, k, m, m, 1.0, 0);
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
                GemmParam<X86, DataType> param(false, false, m, n, k, m, k, m, m, 1.0, 0);
                test_base.setParam(param);
                test_base.setInputShape(input_shapes);
                test_base.runTest(gemm_cpu<DataType, X86, X86>);
            }
        }
    }
#endif

#ifdef USE_HIP
    typedef typename DataTrait<AMD_HIP, DATA_FLOAT>::DType DataType;

    TestAbacusBase<AMDHIPHX86, AMD_HIP, DATA_FLOAT, Gemm, GemmParam> test_base;
    for(int m  : {100, 200, 300}){
        for(int n : {100, 200, 300}){
            for(int k : {100, 200, 300}){
                std::vector<Shape> input_shapes;
                input_shapes.push_back(Shape({1, 1, k, m})); // Matrix A
                input_shapes.push_back(Shape({1, 1, n, k})); // Matrix B
                GemmParam<AMD_HIP, DataType> param(false, false, m, n, k, m, k, m, m, 1.0, 0);
                test_base.setParam(param);
                test_base.setInputShape(input_shapes);
                test_base.runTest(gemm_cpu<DataType, AMDHIPHX86, AMD_HIP>);
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