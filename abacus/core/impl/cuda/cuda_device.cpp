#include "abacus/core/device.h"
#include "ww_config.h"

namespace WW{
namespace abacus{

#ifdef USE_CUDA

template<>
void Device<NV>::getInfo(){
    typedef TargetWrapper<NV> API;
    int device_id = API::getDeviceId();
    cudaDeviceProp device_prop;
    CUDA_CHECK(cudaGetDeviceProperties(&device_prop, device_id));
    _info._idx = device_id;
    _info._device_name = std::string(device_prop.name);

    LOG(INFO) << "Device Id: " << device_id << " , Device name: " << _info.name;
}

template<>
void Device<NV>::createStream(){
    typedef TargetWrapper<NV> API;
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
void Device<NVHX86>::getInfo(){
    LOG(ERROR) << "NVHX86 host getinfo is not implemented!";
}

template<>
void Device<NVHX86>::createStream(){
    LOG(ERROR) << "NVHX86 host createStream is not implemented!";
}

#endif // USE_CUDA

} // namespace abacus
} // namespace WW