#ifndef ABACUS_FUNC_IMPL_HIP_ABACUS_GEMM_H
#define ABACUS_FUNC_IMPL_HIP_ABACUS_GEMM_H

#include <memory>

#include "abacus/func/impl/impl_gemm.h"
#include "abacus/abacus_type.h"
#include "abacus/core/target_wrapper.h"
#include "abacus/core/data_traits.h"
#include "abacus/abacus_func_param.hpp"

namespace WW{
namespace abacus{

template<DATA_TYPE DataType>
class AbacusGemm<AMD_HIP, DataType> : public ImplBase<AMD_HIP, DataType, GemmParam<AMD_HIP, typename DataTrait<AMD_HIP, DataType>::DType>>{
public:
    typedef typename DataTrait<AMD_HIP, DataType>::DType OpDType;
    using Base = ImplBase<AMD_HIP, DataType, GemmParam<AMD_HIP, OpDType>>;
    
    AbacusGemm(){}
    ~AbacusGemm(){}

    ABACUS_STATUS init(Context<AMD_HIP>& ctx, GemmParam<AMD_HIP, OpDType> &param){
        Base::_ctx = std::make_shared<Context<AMD_HIP>>(ctx);
        Base::_param = std::make_shared<GemmParam<AMD_HIP, OpDType>>(param);

        _assemble_name = "Cijk_Ailk_Bjlk_DB_MT128x64x4_SE_K1_PLR1_SNLL1_TT4_4_WG32_16_1_WGM1";

        create();
        return AbacusSuccess;
    }

    ABACUS_STATUS create(){

        // **********************************************************************
        // build assemble code to exe file
        std::string cmd = "../../abacus/func/impl/hip/gcn_assemble/asm.sh ";
        cmd += _assemble_name;
        system(cmd.c_str());
        // **********************************************************************

        // **********************************************************************
        // load exe file to hip function
        /* kernels */
        _hip_function = std::make_shared<hipFunction_t>();
        hipModule_t module = nullptr;
        std::string co_file = "../../abacus/func/impl/hip/gcn_assemble/" + _assemble_name + ".co";
        HIP_CHECK(hipModuleLoad(&module, co_file.c_str()));
        HIP_CHECK(hipModuleGetFunction(&(*_hip_function), module, _assemble_name.c_str()));
        // **********************************************************************
    
        return AbacusSuccess;
    }
    
    ABACUS_STATUS forward(const std::vector<Tensor<AMD_HIP>* >& inputs, std::vector<Tensor<AMD_HIP>*>& outputs){
        
        const unsigned int problemSizes[8] = {64, 64, 1, 64, 64, 64, 64, 64};
        int lda = Base::_param->_lda;
        int ldb = Base::_param->_ldb;
        int ldc = Base::_param->_ldc;
        int ldd = Base::_param->_ldd;

        auto alpha = Base::_param->_alpha;
        auto beta  = Base::_param->_beta;
        
        // calculate parameters assuming packed data
        unsigned int strideD0I = 1;
        unsigned int strideD1J = ldd;
        unsigned int strideD2K = ldd;
        unsigned int strideC0I = 1 ;
        unsigned int strideC1J = ldc;
        unsigned int strideC2K = ldc;
        unsigned int strideA0I = 1;
        unsigned int strideA1L = lda;
        unsigned int strideA2K = lda;
        unsigned int strideB0J = 1;
        unsigned int strideB1L = ldb;
        unsigned int strideB2K = ldb;
        unsigned int sizeI = problemSizes[0];
        unsigned int sizeJ = problemSizes[1];
        unsigned int sizeK = problemSizes[2];
        unsigned int sizeL = problemSizes[3];

        /* module function args */
        struct {
            // Size of Tensor's packed dims, in elements
            uint64_t tensor2dSizeC;
            uint64_t tensor2dSizeA;
            uint64_t tensor2dSizeB;
            float * dataD;
            const float * dataC;
            const float * dataA;
            const float * dataB;
            double alpha;
            double beta;
            unsigned int strideD1J;
            unsigned int strideD2K;
            unsigned int strideC1J;
            unsigned int strideC2K;
            unsigned int strideA1L;
            unsigned int strideA2K;
            unsigned int strideB1L;
            unsigned int strideB2K;
            unsigned int sizeI;
            unsigned int sizeJ;
            unsigned int sizeK;
            unsigned int sizeL;
            int staggerUIter;
            unsigned int problemNumGroupTiles0;
            unsigned int problemNumGroupTiles1;
            unsigned int magicNumberProblemNumGroupTiles0;
            unsigned int gridNumWorkGroups0;
            unsigned int numFullBlocks;
            unsigned int wgmRemainder1;
            unsigned int magicNumberWgmRemainder1;
            unsigned int pad;
        } hipFunctionArgs;
        size_t hipFunctionArgsSize = sizeof(hipFunctionArgs);
        void *hipLaunchParams[] = {HIP_LAUNCH_PARAM_BUFFER_POINTER, &hipFunctionArgs, HIP_LAUNCH_PARAM_BUFFER_SIZE, &hipFunctionArgsSize, HIP_LAUNCH_PARAM_END};

        /* grid sizes */
        const unsigned int workDim = 3;
        const unsigned int threadTile[2] = { 4, 4 };
        const unsigned int groupSize[2] = { 32, 16 };
        size_t localWorkSize[3] = { 512, 1, 1 };
        size_t globalWorkSize[3];
        globalWorkSize[2] = 1;
        globalWorkSize[2] *= sizeK;
        unsigned int sizeOfC0 = sizeI;
        unsigned int sizeOfC1 = sizeJ;
        unsigned int macroTile0 = static_cast<unsigned int>(groupSize[0] * threadTile[0]);
        unsigned int macroTile1 = static_cast<unsigned int>(groupSize[1] * threadTile[1]);
        unsigned int totalWorkGroups0 = sizeOfC0 / macroTile0;
        unsigned int totalWorkGroups1 = sizeOfC1 / macroTile1;

        // b/c single kernel, add extra work-group here if edge needed
        if (totalWorkGroups0*macroTile0 < sizeOfC0) { totalWorkGroups0++; }
        if (totalWorkGroups1*macroTile1 < sizeOfC1) { totalWorkGroups1++; }
        unsigned int problemNumGroupTiles0 = totalWorkGroups0;
        unsigned int problemNumGroupTiles1 = totalWorkGroups1;
        const unsigned smallNumMagicShift = 31; // bozo, review
        unsigned magicNumberProblemNumGroupTiles0 = (1L<<smallNumMagicShift) / problemNumGroupTiles0 + 1; // bozo, review
        unsigned numFullBlocks =  problemNumGroupTiles1 / 1; // divide by WorkGroupMapping
        unsigned wgmRemainder1 =  problemNumGroupTiles1 % 1;
        if (wgmRemainder1 == 0) wgmRemainder1 = 1;
        unsigned magicNumberWgmRemainder1 = ((1L<<smallNumMagicShift) / wgmRemainder1 + 1);
        globalWorkSize[0] = totalWorkGroups0;
        globalWorkSize[1] = totalWorkGroups1;

        /* index sizes */
        uint64_t tensor2dSizeC = 1 * std::max(sizeI, strideC1J) * std::max(sizeJ, strideC2K);
        uint64_t tensor2dSizeA = 1;
        uint64_t tensor2dSizeAStride = 0;
        uint64_t tensor2dSizeAOffset = 0;
        tensor2dSizeAStride = std::max(tensor2dSizeA*sizeI, (uint64_t)strideA1L);
        tensor2dSizeAOffset += tensor2dSizeAStride - tensor2dSizeA*sizeI;
        tensor2dSizeA = tensor2dSizeAStride;
        tensor2dSizeAStride = std::max(tensor2dSizeA*sizeL, (uint64_t)strideA2K);
        tensor2dSizeAOffset += tensor2dSizeAStride - tensor2dSizeA*sizeL;
        tensor2dSizeA = tensor2dSizeAStride;
        tensor2dSizeA -= tensor2dSizeAOffset;

        uint64_t tensor2dSizeB = 1;
        uint64_t tensor2dSizeBStride = 0;
        uint64_t tensor2dSizeBOffset = 0;
        tensor2dSizeBStride = std::max(tensor2dSizeB*sizeJ, (uint64_t)strideB1L);
        tensor2dSizeBOffset += tensor2dSizeBStride - tensor2dSizeB*sizeJ;
        tensor2dSizeB = tensor2dSizeBStride;
        tensor2dSizeBStride = std::max(tensor2dSizeB*sizeL, (uint64_t)strideB2K);
        tensor2dSizeBOffset += tensor2dSizeBStride - tensor2dSizeB*sizeL;
        tensor2dSizeB = tensor2dSizeBStride;
        tensor2dSizeB -= tensor2dSizeBOffset;

        unsigned int staggerUIter = 0; // how many stride-sized clicks to stagger start offset
        int unrollLoopIters = sizeL/4/1; // /DepthU/GSU
        while (staggerUIter>1) {
            if (unrollLoopIters >= (staggerUIter*8)) {
            break;}
            staggerUIter /= 2; // step down to smaller stagger
        }
        if (staggerUIter>=1) staggerUIter -= 1;

        /* kernel 0: Cijk_Ailk_Bjlk_DB_MT128x64x4_SE_K1_PLR1_SNLL1_TT4_4_WG32_16_1_WGM1 */
        unsigned int kernelIdx = 0;
        
        try {
            hipFunctionArgs.tensor2dSizeC = tensor2dSizeC;
            hipFunctionArgs.tensor2dSizeA = tensor2dSizeA;
            hipFunctionArgs.tensor2dSizeB = tensor2dSizeB;
            hipFunctionArgs.dataD = static_cast<float*>(outputs[0]->dataMutable());
            hipFunctionArgs.dataC = static_cast<float*>(outputs[0]->dataMutable());
            hipFunctionArgs.dataA = static_cast<float*>(inputs[0]->dataMutable());
            hipFunctionArgs.dataB = static_cast<float*>(inputs[1]->dataMutable());
            hipFunctionArgs.alpha = alpha;
            hipFunctionArgs.beta = beta;
            hipFunctionArgs.strideD1J = strideD1J;
            hipFunctionArgs.strideD2K = strideD2K;
            hipFunctionArgs.strideC1J = strideC1J;
            hipFunctionArgs.strideC2K = strideC2K;
            hipFunctionArgs.strideA1L = strideA1L;
            hipFunctionArgs.strideA2K = strideA2K;
            hipFunctionArgs.strideB1L = strideB1L;
            hipFunctionArgs.strideB2K = strideB2K;
            hipFunctionArgs.sizeI = sizeI;
            hipFunctionArgs.sizeJ = sizeJ;
            hipFunctionArgs.sizeK = sizeK;
            hipFunctionArgs.sizeL = sizeL;
            hipFunctionArgs.tensor2dSizeC = tensor2dSizeC;
            hipFunctionArgs.tensor2dSizeA = tensor2dSizeA;
            hipFunctionArgs.tensor2dSizeB = tensor2dSizeB;
            hipFunctionArgs.staggerUIter = staggerUIter;

            hipFunctionArgs.problemNumGroupTiles0 = problemNumGroupTiles0;
            hipFunctionArgs.problemNumGroupTiles1 = problemNumGroupTiles1;
            hipFunctionArgs.magicNumberProblemNumGroupTiles0 = magicNumberProblemNumGroupTiles0;
            hipFunctionArgs.gridNumWorkGroups0 = globalWorkSize[0];
            hipFunctionArgs.numFullBlocks = numFullBlocks;
            hipFunctionArgs.wgmRemainder1 = wgmRemainder1;
            hipFunctionArgs.magicNumberWgmRemainder1 = magicNumberWgmRemainder1;
            hipHccModuleLaunchKernel(
                *_hip_function,
                globalWorkSize[0]*localWorkSize[0],
                globalWorkSize[1]*localWorkSize[1],
                globalWorkSize[2]*localWorkSize[2],
                localWorkSize[0],
                localWorkSize[1],
                localWorkSize[2],
                0, // groupMemBytes
                Base::_ctx->getComputeStream(),
                NULL,
                (void**)hipLaunchParams,
                nullptr,
                nullptr
            );
        } catch (const std::exception& e) {
            return AbacusUnknowError;
        }
        
        return AbacusSuccess;
    }


private:
    std::string _assemble_name;
    std::shared_ptr<hipFunction_t> _hip_function;
};

} // namespace abacus
} //namespace WW
#endif