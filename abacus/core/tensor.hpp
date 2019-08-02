#ifndef ABACUS_CORE_TENSOR_HPP
#define ABACUS_CORE_TENSOR_HPP

#include <memory>
#include "abacus/core/buffer.hpp"
#include "abacus/core/shape.hpp"
#include "abacus/abacus_type.h"
#include "abacus/core/common.h"


namespace WW{
namespace abacus{

template<typename TargetType>
class Tensor{
public:
    typedef typename Buffer<TargetType>::PtrDType PtrDType;
    typedef TargetWrapper<TargetType> API;
    typedef typename TargetTraits<TargetType>::TargetType target_type;
    typedef typename TargetTraits<TargetType>::TargetCategory target_category;

    Tensor(DATA_TYPE type = DATA_FLOAT)
        :_data_type(type),
         _type_len(typeByteNum(type)),
         _shape(){
    }

    Tensor(Shape shape, DATA_TYPE type = DATA_FLOAT)
        :_data_type(type),
         _type_len(typeByteNum(type)),
         _shape(shape){

        _data = std::make_shared<Buffer<TargetType>>(shape.count() * _type_len);
        _diff = std::make_shared<Buffer<TargetType>>(shape.count() * _type_len);
    }

    Tensor(const Tensor<TargetType>& rhs)
        :_shape(rhs._shape),
         _data_type(rhs._data_type),
         _type_len(rhs._type_len),
         _data(rhs._data),
         _diff(rhs._diff){}


    Tensor<TargetType>& operator= (const Tensor<TargetType>& rhs){
        if(this == &rhs) return *this;
        clear();
        this->_shape = rhs._shape;
        this->_data_type = rhs._data_type;
        this->_type_len = rhs._type_len;
        this->_data = rhs._data;
        this->_diff = rhs._diff;

        return *this;
    }


    ABACUS_STATUS setDataType(DATA_TYPE type){
        CHECK_EQ(type == DATA_INVALID || type == DATA_VOID, false) \
            << "DataType is invalid, set DataType failed!";

        _data_type = type;
        _type_len  = typeByteNum(type);
        
        if(_data->capacity() < _type_len * _shape.count()){

            if(_is_shared){
                LOG(ERROR) << "tensor is shared, memory can not be re-alloced.";
                return AbacusOutofAuthority;
            }
            
            _data->realloc(_type_len * _shape.count());
            _diff->realloc(_type_len * _shape.count());
        }

        return AbacusSuccess;
    }


    //brief free old buff and alloc a new buff.
    ABACUS_STATUS realloc(Shape shape, DATA_TYPE type = DATA_INVALID){
        CHECK_EQ(_is_shared, true) \
            << "shared tensor could not realloc.";
        CHECK_EQ(type == DATA_VOID, false) \
            << "DataType is invalid, realloc failed!";
        
        if(DATA_INVALID != type){
            _data_type = type;
            _type_len  = typeByteNum(type);    
        }
        
        _shape = shape;
        _data->alloc(_shape.count() * _type_len);
        _diff->alloc(_shape.count() * _type_len);
        return AbacusSuccess;
    }

    //brief change the tensor shape
    // if new shape bigger than the old, alloc a new buff.
    ABACUS_STATUS reshape(Shape shape){
        CHECK_EQ(shape > Shape::zero(shape), true) << \
            "shape should be bigger than 0.";
        _shape = shape;
        
        bool exceed_flag = (_shape.count() * _type_len > _data->capacity()) && _is_shared;
        CHECK_EQ(exceed_flag, false) << "shared tensor shape exceed origin data buffer size";
        ABACUS_CHECK(_data->realloc(_shape.count() * _type_len));
        return AbacusSuccess;
    }

    const PtrDType data() const{
        CHECK_EQ(deviceId(), API::getDeviceId()) \
            << "tensor is not declared in current device.";
        
        if(_data->capacity() == 0) return nullptr;
        return static_cast<PtrDType>(_data->getDataMutable());
    }

    PtrDType dataMutable(){
        CHECK_EQ(deviceId(), API::getDeviceId()) \
            << "tensor is not declared in current device.";
        
        if(_data->capacity() == 0) return nullptr;
        return static_cast<PtrDType>(_data->getDataMutable());
    }

    const PtrDType diff() const{
        CHECK_EQ(deviceId(), API::getDeviceId()) \
            << "tensor is not declared in current device.";
        
        if(_diff->capacity() == 0) return nullptr;
        return static_cast<PtrDType>(_diff->getDataMutable());
    }

    PtrDType diffMutable(){
        CHECK_EQ(deviceId(), API::getDeviceId()) \
            << "tensor is not declared in current device.";
        
        if(_diff->capacity() == 0) return nullptr;
        return static_cast<PtrDType>(_diff->getDataMutable());
    }

    long long count(int start, int end) const{
        return _shape.count(start, end);
    }

    long long size() const{
        return _shape.count();
    }

    int dims() const{
        return _shape.dims();
    }

    const Shape shape() const{
        return _shape;
    }

    const size_t getDataTypeSize() const{
        return _type_len;
    }
    
    const int batch() const{
        return _shape.batch();
    }

    const int batchIndex() const{
        return _shape.batchIndex();
    }

    const int channel() const{
        return _shape.channel();
    }

    const int channelIndex() const{
        return _shape.channelIndex();
    }

    const int height() const{
        return _shape.height();
    }
    
    const int heightIndex() const{
        return _shape.heightIndex();
    }

    const int width() const{
        return _shape.width();
    }

    const int widthIndex() const{
        return _shape.widthIndex();
    }

    const int deviceId() const{
        return _data->getDeviceId();
    }

    const LAYOUT_TYPE layout() const{
        return _shape.getLayoutType();
    }

    const DATA_TYPE dataType() const{
        return _data_type;
    }


    // deep copy from other tensor
    template <typename TargetType_t>
    ABACUS_STATUS copyFrom(const Tensor<TargetType_t>& tensor){
        CHECK_EQ(tensor.dataType(), _data_type)\
            << "data type should be the same.";
        CHECK_EQ(tensor.size(), size()) \
            << "size of two shapes must be the same.";


        typedef TargetWrapper<TargetType_t> API_t;
        typedef typename TargetTraits<TargetType_t>::TargetType target_type_t;
        typedef typename IFELSE<std::is_same<target_type, target_type_t>::value, __H2H, __D2H>::TYPE ThenType;
        typedef typename IFELSE<std::is_same<target_type, target_type_t>::value, __D2D, __H2D>::TYPE ElseType;
        typedef typename IFELSE<std::is_same<__HOST_TARGET, target_category>::value, ThenType, ElseType>::TYPE DirectionKind;
        typedef typename IFELSE<std::is_same<__HOST_TARGET, target_category>::value, API_t, API>::TYPE SELECTED_API;
        
        if(std::is_same<TargetType, TargetType_t>::value){
            if(data() == tensor.data()) return AbacusSuccess;
        }


        SELECTED_API::syncMemcpy(_data->getDataMutable(), 0, deviceId(), tensor.data(), 0, tensor.deviceId(), tensor.size() * _type_len, DirectionKind());
        SELECTED_API::syncMemcpy(_diff->getDataMutable(), 0, deviceId(), tensor.diff(), 0, tensor.deviceId(), tensor.size() * _type_len, DirectionKind());

        return AbacusSuccess;
    }

    template<typename TargetType_t, typename StreamType = \
        typename IFELSE<std::is_same<__HOST_TARGET, target_category>::value, \
            typename TargetWrapper<TargetType_t>::stream_t, typename TargetWrapper<TargetType>::stream_t>::TYPE>
    ABACUS_STATUS copyFromAsync(const Tensor<TargetType_t>& tensor, StreamType stream){
        CHECK_EQ(tensor.dataType(), _data_type)\
            << "data type should be the same.";
        CHECK_EQ(tensor.size(), size()) \
            << "size of two shapes must be the same.";

        typedef TargetWrapper<TargetType_t> API_t;
        typedef typename TargetTraits<TargetType_t>::TargetType target_type_t;
        typedef typename IFELSE<std::is_same<target_type, target_type_t>::value, __H2H, __D2H>::TYPE ThenType;
        typedef typename IFELSE<std::is_same<target_type, target_type_t>::value, __D2D, __H2D>::TYPE ElseType;
        typedef typename IFELSE<std::is_same<__HOST_TARGET, target_category>::value, ThenType, ElseType>::TYPE DirectionKind;
        typedef typename IFELSE<std::is_same<__HOST_TARGET, target_category>::value, API_t, API>::TYPE SELECTED_API;
        
        if(std::is_same<TargetType, TargetType_t>::value){
            if(data() == tensor.data()) return AbacusSuccess;
        }


        SELECTED_API::asyncMemcpy(_data->getDataMutable(), 0, deviceId(), tensor.data(), 0, tensor.deviceId(), tensor.size() * _type_len, stream, DirectionKind());
        SELECTED_API::asyncMemcpy(_diff->getDataMutable(), 0, deviceId(), tensor.diff(), 0, tensor.deviceId(), tensor.size() * _type_len, stream, DirectionKind());

        return AbacusSuccess;
    }

private:
    bool _is_shared{false};
    
    DATA_TYPE _data_type{DATA_FLOAT};
    size_t    _type_len{4};
    
    Shape _shape;
    std::shared_ptr<Buffer<TargetType>> _data{nullptr};
    std::shared_ptr<Buffer<TargetType>> _diff{nullptr};

    void clear(){
        if(_data) _data.reset();
        if(_diff) _diff.reset();
    }
};



} // namespace abacus
} // namespace WW


#endif