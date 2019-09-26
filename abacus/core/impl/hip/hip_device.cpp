#include "abacus/core/device.h"
#include "ww_config.h"

namespace WW{
namespace abacus{

#ifdef USE_HIP

template<>
void Device<AMD_HIP>::getInfo(){
    typedef TargetWrapper<AMD_HIP> API;
    int device_id = API::getDeviceId();
    hipDeviceProp_t device_prop;
    HIP_CHECK(hipGetDeviceProperties(&device_prop, device_id));
    _info._idx = device_id;
    _info._device_name = std::string(device_prop.name);

    LOG(INFO) << "Device Id: " << device_id << " , Device name: " << _info._device_name;
}

template<>
void Device<AMD_HIP>::createStream(){
    typedef TargetWrapper<AMD_HIP> API;
    typename API::stream_t stream_data;
    typename API::stream_t stream_compute;

    _data_stream.clear();
    _compute_stream.clear();
    
    for(int i = 0; i < _max_stream; ++i){
        API::createStreamWithFlag(&stream_data, 1);
        API::createStreamWithFlag(&stream_compute, 1);
    
        _data_stream.emplace_back(stream_data);
        _compute_stream.emplace_back(stream_compute);    
    }
}

template<>
void Device<AMDHIPHX86>::getInfo(){
    LOG(ERROR) << "AMDHIPHX86 host getinfo is not implemented!";
}

template<>
void Device<AMDHIPHX86>::createStream(){
    LOG(ERROR) << "AMDHIPHX86 host createStream is not implemented!";
}

#endif // USE_HIP

} // namespace abacus
} // namespace WW