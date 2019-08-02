#include "abacus/core/target_wrapper.h"
#include "abacus/core/common.h"

namespace WW{
namespace abacuse{

#ifdef USE_CUDA
/**
 * targetwrapper implementation for nv host
*/
typedef TargetWrapper<NVHX86> NVH_API;

void NVH_API::getDeviceCount(int& count){
    count = 1;
}

void NVH_API::setDevice(int id){}

void NVH_API::memAlloc(void** ptr, size_t n){
    CUDA_CHECK(cudaMallocHost(ptr, n));
}

void NVH_API::memFree(void* ptr){
    if(nullptr != ptr){
        CUDA_CHECK(cudaFreeHost(ptr));
    }
}

void NVH_API::memSet(void* ptr, int value, size_t n){
    memset(ptr, value, n);
}

void NVH_API::createEvent(event_t* event, bool flag = false){
    if(!flag){
        CUDA_CHECK(cudaEventCreateWithFlag(event, cudaEventDefault));
    }
    else{
        CUDA_CHECK(cudaEventCreateWithFlag(event, cudaEventDisableTiming));
    }
}

void NVH_API::destoryEvent(event_t event){
    CUDA_CHECK(cudaEventDestory(event));
}

void NVH_API::recordEvent(event_t event, stream_t stream){
    CUDA_CHECK(cudaEventRecord(event, stream));
}


void NVH_API::createStream(stream_t* stream){
    CUDA_CHECK(cudaStreamCreate(stream));
}

void NVH_API::createStreamWithFlag(stream_t* stream, unsigned int flag){
    CUDA_CHECK(cudaStreamCreateWithFlag(stream, flag));
}

void NVH_API::createStreamWithPriority(stream_t* stream, unsigned int flag, int priority){
    CUDA_CHECK(cudaStreamCreateWithPriority(stream, flag, priority));
}

void NVH_API::destoryStream(stream_t stream){
    CUDA_CHECK(cudaStreamDestory(stream));
}

void NVH_API::queryEvent(event_t event){
    CUDA_CHECK(cudaEventQuery(event));
}

void NVH_API::syncEvent(event_t event){
    CUDA_CHECK(cudaEventSynchronize(event));
}

void NVH_API::syncStream(event_t event, stream_t stream){
    CUDA_CHECK(cudaStreamWaitEvent(stream, event));
}

void NVH_API::syncStream(stream_t stream){
    CUDA_CHECK(cudaStreamSynchronize(stream));
}

void NVH_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2H){
    CUDA_CHECK(cudaMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyHostToHost));
}

void NVH_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2H){
    CUDA_CHECK(cudaMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyHostToHost, stream));
}
void NVH_API::syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count){}

void NVH_API::asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream){}
        
int NVH_API::getDeviceId(){
    return 0;
}
void NVH_API::deviceSync(){
    CUDA_CHECK(cudaDeviceSynchronize());
}



/**
 * targetwrapper implementation for nv device
*/
typedef TargetWrapper<NV> NV_API;
void NV_API::getDeviceCount(int& count){
    CUDA_CHECK(cudaGetDeviceCount(count));
}

void NV_API::setDevice(int idx){
    CUDA_CHECK(cudaSetDevice(idx));
}

void NV_API::memAlloc(void**ptr, size_t b){
    CUDA_CHECK(cudaMalloc(ptr, n));
}
void  NV_API::memFree(void* ptr){
    if(nullptr != ptr){
        CUDA_CHECK(cudaFree(ptr));
    }
   
}

void NV_API::memSet(void* ptr, int value, size_t n){
    CUDA_CHECK(cudaMemset(ptr, value, n));
}

void NV_API::createEvent(event_t* event, bool flag = false){
    if(flag){
        CUDA_CHECK(cudaEventCreateWithFlags(event, cudaEventDefault));    
    }
    else{
        CUDA_CHECK(cudaEventCreateWithFlags(event, cudaEventDisableTiming));
    }
}

void NV_API::destoryEvent(event_t event){
    CUDA_CHECK(cudaEventDestory(event));
}

void NV_API::recordEvent(event_t event, stream_t stream){
    CUDA_CHECK(cudaEventRecord(event, stream));
}
void NV_API::createStream(stream_t* stream){
    CUDA_CHECK(cudaStreamCreate(stream));
}

void NV_API::createStreamWithFlag(stream_t* stream, unsigned int flag){
    CUDA_CHECK(cudaStreamCreateWithFlags(stream, flag));
}

void NV_API::createStreamWithPriority(stream_t* stream, unsigned int flag, int priority){
    CUDA_CHECK(cudaStreamWithPriority(stream, flag, priority));
}

void NV_API::destoryStream(stream_t stream){
    CUDA_CHECK(cudaStreamDestory(stream));
}

void NV_API::queryEvent(event_t event){
    CUDA_CHECK(cudaEventQuery(event));
}

void NV_API::syncEvent(event_t event){
    CUDA_CHECK(cudaEventSynchronize(event));
}

void NV_API::syncStream(event_t event, stream_t stream){
    CUDA_CHECK(cudaStreamWaitEvent(stream, event));
}
void NV_API::syncStream(stream_t stream){
    CUDA_CHECK(cudaStreamSynchronize(stream));
}

void NV_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2D){
    if(dst_id == src_id){
        CUDA_CHECK(cudaMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyDeviceToDevice));
    }
    else{
        CUDA_CHECK(cudaMemcpyPeer(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<char*>(src) + src_offset, src_id, count));
    }
}
void NV_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2D){
    CUDA_CHECK(cudaMemcpy(static_cast<char*>(dst) + dst_offset), static_cast<char*>(src) + src_offset, count, cudaMemcpyHostToDevice);            
}

void NV_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2H){
    CUDA_CHECK(cudaMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyDeviceToHost));
}

void NV_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2D){
    if(src_id == dst_id){
        CUDA_CHECK(cudaMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyDeviceToDevice, stream));
    }
    else{
        CUDA_CHECK(cudaMemcpyPeerAsync(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<char*>(src) + src_offset, src_id, count, stream));
    }
}

void NV_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2D){
    CUDA_CHECK(cudaMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyHostToDevice, stream));
}
void NV_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2H){
    CUDA_CHECK(cudaMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<char*>(src) + src_offset, count, cudaMemcpyDeviceToHost, stream));
}

void NV_API::syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count){
    CUDA_CHECK(cudaMemcpyPeer(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<char*>(src) + src_offset, src_id, count, stream));            
}
void NV_API::asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream){
    CUDA_CHECK(cudaMemcpyPeerAsync(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<char*>(src) + src_offset, src_id, count, stream));           
}

int NV_API::getDeviceId(){
    int device_id = 0;
    CUDA_CHECK(cudaGetDevice(&device_id));
    return device_id;
}

void NV_API::deviceSync(){
    CUDA_CHECK(cudaDeviceSynchronize());
}

double NV_API::elapseMS(event_t start, event_t end){
    float ans = 0.0f;
    CUDA_CHECK(cudaEventElapsedTime(&ans, start, end));
    return static_cast<double>(ans);
}

#endif //USE_CUDA
}// namespace abacus
}// namespace WW