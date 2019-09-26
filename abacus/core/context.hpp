#ifndef ABACUS_CORE_CONTEXT_HPP
#define ABACUS_CORE_CONTEXT_HPP
#include "abacus/core/env.hpp"

namespace WW{
namespace abacus{

template<typename TargetType>
class Context{
public:
    typedef TargetWrapper<TargetType> API;
    
    Context(int device_id = 0, int data_stream_id = 0, int compute_stream_id = 0){
        typename Env<TargetType>::Devs& devs = Env<TargetType>::getEnv();
        
        CHECK_GT(devs.size(), 0) << "Env is not initialized or no valid device(s).";
        CHECK_LT(device_id, devs.size()) << "Device idx exceeds the num of devices.";
        
        Device<TargetType>& dev = devs[device_id];
        if(dev.getComputeStream().empty() ||
           dev.getDataStream().empty()){
            dev.createStream();
        }

        CHECK_LT(data_stream_id, dev.getDataStream().size()) << "Data stream id exceeds the max data stream.";
        CHECK_LT(compute_stream_id, dev.getComputeStream().size()) << "Compute stream id exceeds the max compute stream.";
        

        _device_id         = device_id;
        _stream_compute_id = compute_stream_id;
        _stream_data_id    = data_stream_id;
        _stream_data = dev.getDataStream()[data_stream_id];
        _stream_compute = dev.getComputeStream()[compute_stream_id];
    }

    Context(const Context<TargetType>& rhs){
        _device_id         = rhs._device_id;
        _stream_data_id    = rhs._stream_data_id;
        _stream_compute_id = rhs._stream_compute_id;
        _stream_data       = rhs._stream_data;
        _stream_compute    = rhs._stream_compute;
    }

    Context& operator = (const Context<TargetType>& rhs){
        if(this == &rhs) return *this;
        
        this->_device_id         = rhs._device_id;
        this->_stream_data_id    = rhs._stream_data_id;
        this->_stream_compute_id = rhs._stream_compute_id;
        this->_stream_data       = rhs._stream_data;
        this->_stream_compute    = rhs._stream_compute;
    }

    bool operator == (const Context<TargetType>& rhs){
        if(this == &rhs) return true;
        
        if(this->_device_id         == rhs._device_id        &&
           this->_stream_data_id    == rhs._stream_data_id   &&
           this->_stream_compute_id == rhs._stream_compute_id) return true;
        else return false;
    }

    const int getDeviceId() const{
        return _device_id;
    }
    
    const int getDataStreamId() const{
        return _stream_data_id;
    }
    
    const int getComputeStreamId() const{
        return _stream_compute_id;
    }

    const typename API::stream_t getDataStream() const{
        return _stream_data;
    }

    const typename API::stream_t getComputeStream() const{
        return _stream_compute;
    }
    
private:
    int _device_id;
    int _stream_data_id;
    int _stream_compute_id;
    typename API::stream_t _stream_data;
    typename API::stream_t _stream_compute;
    
};


// --------------------------------------
// Context X86 implement.
// --------------------------------------
template<>
class Context<X86>{
public:
    typedef TargetWrapper<X86> API;
    
    Context(int device_id = 0, int data_stream_id = 0, int compute_stream_id = 0){
        typename Env<X86>::Devs& devs = Env<X86>::getEnv();
        
        CHECK_GT(devs.size(), 0) << "Env is not initialized or no valid device(s).";
        CHECK_LT(device_id, devs.size()) << "Device idx exceeds the num of devices.";
        
        _device_id = device_id;
    }

    Context(const Context<X86>& rhs){
        _device_id         = rhs._device_id;
    }

    Context& operator = (const Context<X86>& rhs){
        if(this == &rhs) return *this;
        
        this->_device_id = rhs._device_id;
        return *this;
    }

    bool operator == (const Context<X86>& rhs){
        if(this == &rhs) return true;
        
        if(this->_device_id == rhs._device_id) return true;
        else return false;
    }

    const int getDeviceId() const{
        return _device_id;
    }
    
private:
    int _device_id;
};
} // namespace abacus
} // namespace WW
#endif