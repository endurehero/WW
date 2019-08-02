#ifndef ABACUS_CORE_DEVICE_H
#define ABACUS_CORE_DEVICE_H

#include <vector>
#include <string>
#include "abacus/core/target_wrapper.h"

namespace WW{
namespace abacus{

template<typename TargetType>
class DeviceInfo{
public:
    int _idx;
    std::string _device_name;
};

template<typename TargetType>
class Device{
public:
    typedef TargetWrapper<TargetType> API;
    Device(int max_stream = 4)
        :_max_stream(max_stream){
        getInfo();        
    }

    void getInfo();
    void createStream();
    
    const int getMaxStream() const{
        return _max_stream;
    }
    
    const DeviceInfo<TargetType>& getDeviceInfo() const{
        return _info;
    }

    const std::vector<typename API::stream_t>& getDataStream() const{
        return _data_stream;
    }

    std::vector<typename API::stream_t>& getDataStream(){
        return _data_stream;
    }

    const std::vector<typename API::stream_t>& getComputeStream() const{
        return _compute_stream;
    }

    std::vector<typename API::stream_t>& getComputeStream(){
        return _compute_stream;
    }
private:
    int _max_stream;
    DeviceInfo<TargetType> _info;
    std::vector<typename API::stream_t> _data_stream;
    std::vector<typename API::stream_t> _compute_stream;
};
} // namespace abacus
} // namespace WW
#endif