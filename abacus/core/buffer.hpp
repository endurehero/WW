#ifndef ABACUS_CORE_BUFFER_HPP
#define ABACUS_CORE_BUFFER_HPP

#include "abacus/core/target_wrapper.h"
#include "abacus/core/data_traits.h"
#include "abacus/core/common.h"

namespace WW{
namespace abacus{

template<typename TargetType>
class Buffer{
public:
    typedef typename DataTrait<TargetType, DATA_VOID>::PtrDType PtrDType;
    typedef TargetWrapper<TargetType> API;

    Buffer() :_device_id(API::getDeviceId()){}
    
    explicit Buffer(size_t size)
        :_size(size),
         _capacity(size),
         _device_id(API::getDeviceId()){
        
        ABACUS_CHECK(alloc(size));
    }

    Buffer(PtrDType data, size_t size, int device_id)
        :_data(data),
         _size(size),
         _capacity(size),
         _device_id(device_id),
         _own(false){

        int cur_device_id = API::getDeviceId();
        CHECK_EQ(_device_id, cur_device_id) << "Data is not in current device.";
    }

    Buffer(const Buffer<TargetType>& rhs){
        CHECK_GT(rhs.size(), 0) << "input buffer is empty()";
        _device_id = API::getDeviceId();
        
        if(_device_id == rhs.getDeviceId()){
            _size = _capacity = rhs.size();
            _own = false;
            _data = rhs.getData();
        }
        else{
            _own = true;
            ABACUS_CHECK(alloc(_size));
            API::syncMemcpyP2p(_data, 0, _device_id, rhs.getData(), 0, rhs.getDeviceId(), _size);
        }
    }

    Buffer<TargetType>& operator = (const Buffer<TargetType>& rhs){
        if(this == &rhs) return *this;
        CHECK_GT(rhs.size(), 0) << "input buffer is empty()";
        clear();
        this->_device_id = API::getDeviceId();
        if(this->_device_id == rhs.getDeviceId()){
            this->_size = this->_capacity = rhs.size();
            this->_own = false;
            this->_data = rhs.getData();
        }
        else{
            this->_own = true;
            ABACUS_CHECK(realloc(this->_size));
            API::syncMemcpyP2p(this->_data, 0, this->_device_id, rhs.getData(), 0, rhs.getDeviceId(), this->_size);
        }

        return *this;
    }

    ~Buffer(){
        clear();
    }

    const int getDeviceId() const{
        return _device_id;
    }
    
    const size_t size() const{
        return _size;
    }

    const size_t capacity() const{
        return _capacity;
    }

    const PtrDType getData() const{
        return _data;
    }

    PtrDType getDataMutable() const{
        return _data;
    }

    ABACUS_STATUS alloc(size_t size){
        clear();
        API::memAlloc(&_data, size);
        _size = _capacity = size;
        _own = true;
        return AbacusSuccess;
    }

    ABACUS_STATUS realloc(size_t size){
        if(_size > _capacity){
            if(_own){
                CHECK_EQ(_device_id, API::getDeviceId()) \
                    << "Buffer is not declared in current device. Could not re_alloc buffer.";
                
                clear();
                API::memAlloc(&_data, size);
                _capacity = size;
            }
            else{
                return AbacusOutofAuthority;
            }
        }
        
        _size = size;
        return AbacusSuccess;
    }

private:
    int _device_id;
    PtrDType _data{nullptr};
    size_t _size{0};
    size_t _capacity{0};
    bool _own{true};

    void clear(){
        if(_own && _capacity > 0){
            _size = 0;
            _capacity = 0;
            _own = true;
            API::memFree(_data);
        }

        _data = nullptr;
    }


};


} // namespace abacus
} // namespace WW

#endif