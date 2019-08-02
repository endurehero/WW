#ifndef ABACUS_FUNC_IMPL_IMPL_COMMON_H
#define ABACUS_FUNC_IMPL_IMPL_COMMON_H

#include "abacus/abacus_type.h"
#include "abacus/func/impl/impl_base.hpp"
#include "abacus/core/data_traits.h"
#include "abacus/abacus_func_param.hpp"

#define DEFINE_OP_CLASS(class_name, param_name) \
template <typename TargetType, DATA_TYPE DataType = DATA_FLOAT> \
class Abacus##class_name: public ImplBase<TargetType, DataType, param_name<TargetType, typename DataTrait<TargetType, DataType>::DType>>{};



#endif