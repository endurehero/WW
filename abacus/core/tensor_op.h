#ifndef ABACUS_CORE_TENSOR_OP_H
#define ABACUS_CORE_TENSOR_OP_H

#include "abacus/core/tensor.hpp"

namespace WW{
namespace abacus{

extern const float eps;

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDataHostConst(Tensor<TargetType>& tensor, float val, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDiffHostConst(Tensor<TargetType>& tensor, float val, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDataHostRand(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType, DATA_TYPE DataT>
void fillTensorDiffHostRand(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType>
void fillTensorDataHostRand(Tensor<TargetType>& tensor, float val_start, float val_end, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType>
void fillTensorDiffHostRand(Tensor<TargetType>& tensor, float val_start, float val_end, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType, DATA_TYPE DataT>
void printTensorHostData(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename TargetType, DATA_TYPE DataT>
void printTensorHostDiff(Tensor<TargetType>& tensor, typename Tensor<TargetType>::API::stream_t stream = NULL);

template<typename DataType>
void tensorCmpHost(const DataType* src1, const DataType* src2, size_t size, double& max_ratio, double& max_diff);


} // namespace abacus
} // namespace WW
#endif