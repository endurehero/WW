#ifndef TEST_ABACUS_TEST_ABACUS_BASE_HPP
#define TEST_ABACUS_TEST_ABACUS_BASE_HPP

#include <exception>

#include "abacus/func/func_base.hpp"
#include "abacus/abacus_type.h"
#include "abacus/abacus_func_param.hpp"
#include "abacus/core/data_traits.h"
#include "abacus/core/tensor_op.h"
#include "abacus/core/context.hpp"
#include "abacus/core/env.hpp"
#include "test/abacus/test_abacus_func.hpp"
#include "utils/unit_test/test_suite.h"
#include "utils/log/logger.h"
#include "utils/timer.hpp"

using namespace WW::abacus;

namespace WW{
namespace test{

template<typename TargetType_H, typename TargetType_D, DATA_TYPE DataType,
         template<typename T, DATA_TYPE D> class Op,
         template<typename T, typename DataT> class Param>
class TestAbacusBase{
public:
    typedef Op<TargetType_D, DataType> op_t;
    typedef Tensor<TargetType_H> TensorH;
    typedef Tensor<TargetType_D> TensorD;
    typedef std::vector<TensorD*> DataVecD;
    typedef std::vector<TensorH*> DataVecH;
    typedef typename DataTrait<TargetType_H, DataType>::DType OpDataType;
    typedef Param<TargetType_D, OpDataType> param_t;
    typedef void (*cpufunc_t)(const DataVecH&, DataVecH&, const param_t&);

    TestAbacusBase(){}
    ~TestAbacusBase(){
        clearData();
    }
    
    void runTest(const cpufunc_t& cpu_func, double succ_ratio = 0.00001, bool save_error_tensor = false, bool profile = false){
        fillInputs();
        Env<TargetType_H>::envInit();
        Env<TargetType_D>::envInit();

        getOpResult(0, profile);
        getCpuResult(cpu_func);
        resultCheckAccuracy(succ_ratio, save_error_tensor);
    }

    
    void setParam(const param_t& param){
        _params.clear();
        addParam(param);
    }

    void addParam(const param_t& param){
        _params.emplace_back(param);
    }

    void setRandLimits(float min_val, float max_val){
        _max_val = max_val;
        _min_val = min_val;
    }

    void setSpecialVal(OpDataType val){
        _special_value = val;
    }

    void setInputShape(const std::vector<Shape>& shapes, int param_id = 0){
        CHECK_GT(shapes.size(), 0) << "size of shapes must be positve!";
        CHECK_GE(param_id, 0) << "param id must >= 0";
        CHECK_LT(param_id, _params.size()) << "param id exceed the max num.";

        clearData();
        
        for(int i = 0; i < shapes.size(); ++i){
            TensorH* input_host = createTensor<TensorH>(shapes[i]);
            TensorD* input_dev  = createTensor<TensorD>(shapes[i]);

            _inputs_host.emplace_back(input_host);
            _inputs_dev.emplace_back(input_dev);
        }

        std::vector<Shape> out_shapes;
        ABACUS_CHECK(_op.computeOutputShape(_inputs_dev, _params[param_id], out_shapes));
        
        for(int i = 0; i < out_shapes.size(); ++i){
            TensorH* output_host = createTensor<TensorH>(out_shapes[i]);
            TensorH* output_dev_host = createTensor<TensorH>(out_shapes[i]);
            TensorD* output_dev  = createTensor<TensorD>(out_shapes[i]);

            _outputs_host.emplace_back(output_host);
            _outputs_dev_host.emplace_back(output_dev_host);
            _outputs_dev.emplace_back(output_dev);
        }
    }
    
    
private:
    TEST_DATA_TYPE _input_type{RANDOM};
    OpDataType _special_value{0};
    float _max_val{1.0};
    float _min_val{-1.0};

    std::vector<param_t> _params;
    DataVecH _inputs_host;
    DataVecD _inputs_dev;
    
    DataVecH _outputs_host;
    DataVecH _outputs_dev_host;
    DataVecD _outputs_dev;
    
    op_t _op;

    template<typename TensorType>
    void clearTensorVec(std::vector<TensorType*>& data_vec){
        for(auto mem : data_vec){
            if(nullptr != mem){
                delete mem;
                mem = nullptr;
            }
        }

        data_vec.clear();
    }

    template<typename TensorType>
    TensorType* createTensor(Shape shape){
        TensorType* ptr = nullptr;
        try{
            ptr = new TensorType(shape, DataType);
        }
        catch(const std::bad_alloc& e){
            LOG(ERROR) << "Tensor alloced failed!";
            exit(1);
        };

        return ptr;
    }

    void clearData(){
        clearTensorVec<TensorD>(_inputs_dev);
        clearTensorVec<TensorD>(_outputs_dev);
        
        clearTensorVec<TensorH>(_inputs_host);
        clearTensorVec<TensorH>(_outputs_host);
        clearTensorVec<TensorH>(_outputs_dev_host);
    }


    void getOpResult(int param_id = 0, bool profile = false){
        CHECK_GE(param_id, 0) << "param index must be positive.";
        CHECK_LT(param_id, _params.size()) << "param index out of range.";
        
        Context<TargetType_D> ctx(0, 1, 1);
        ABACUS_STATUS status;
        
        Timer<TargetType_D> t(ctx);
        int iter_num = profile ? 100 : 1;
        
        t.clear();

        _op.init(ctx, _params[param_id]);

        for(int i = 0; i < _inputs_host.size(); ++i){
            _inputs_dev[i]->reshape(_inputs_host[i]->shape());
            _inputs_dev[i]->copyFrom(*(_inputs_host[i]));
        }
        
        for(int itr = 1; itr <= iter_num; ++itr){
            t.start();
            _op(_inputs_dev, _outputs_dev);
            t.end();
        }

        float ts = t.getAvgElapsed();
        if(profile){
            LOG(INFO) << "avg run time: " << ts << "ms";
        }
        
        for(int i = 0; i < _outputs_dev_host.size(); ++i){
            _outputs_dev_host[i]->reshape(_outputs_dev[i]->shape());
            _outputs_dev_host[i]->copyFrom(*(_outputs_dev[i]));
        }
    }

    void getCpuResult(const cpufunc_t& cpu_func, int param_id = 0){
        CHECK_EQ(cpu_func != nullptr, true) << "Cpu func does not implement!";
        CHECK_GE(param_id, 0) << "Param index must be positive";
        CHECK_LT(param_id, _params.size()) << "Param index out of range!";
        cpu_func(_inputs_host, _outputs_host, _params[param_id]);
    }

    void resultCheckAccuracy(double succ_ratio = 0.00001, bool save_error_tensor = false){
        CHECK_EQ(_outputs_host.size(), _outputs_dev_host.size()) \
            << "output size in device and cpu must be equal！";
        
        int check_size = _outputs_host.size();
        std::vector<double> max_diff(check_size, 0);
        std::vector<double> max_ratio(check_size, 0);

        for(int i = 0; i < check_size; ++i){
            tensorCmpHost<OpDataType>(static_cast<const OpDataType*>(_outputs_host[i]->data()), \
                                      static_cast<const OpDataType*>(_outputs_dev_host[i]->data()),\
                                      _outputs_host[i]->size(), max_ratio[i], max_diff[i]);
            
            if(max_diff[i] < 0.0001 || max_ratio[i] < succ_ratio &&
               _outputs_host[i]->shape() == _outputs_dev_host[i]->shape()){
                LOG(INFO) << "Test Passed!";
            }
            else{
                LOG(INFO) << "max_ratio: " << max_ratio[i] << ", max_diff: " << max_diff[i];
                LOG(ERROR) << "Test Failed!" << "output: " << i;
            }
        }

        
    }

    void fillInputs(){
        if(_input_type == RANDOM){
            for(int i = 0; i < _inputs_host.size(); ++i){
                fillTensorDataHostRand<TargetType_H, DataType>(*(_inputs_host[i]));
                fillTensorDiffHostRand<TargetType_H, DataType>(*(_inputs_host[i]));
                _inputs_dev[i]->copyFrom(*(_inputs_host[i]));
            }
        }
        else{
            for(int i = 0; i < _inputs_host.size(); ++i){
                fillTensorDataHostConst<TargetType_H, DataType>(*(_inputs_host[i]), _special_value);
                fillTensorDiffHostConst<TargetType_H, DataType>(*(_inputs_host[i]), _special_value);
                _inputs_dev[i]->copyFrom(*(_inputs_host[i]));
            }
        }
    }

};
} // namespace test
} // namespace WW

#endif