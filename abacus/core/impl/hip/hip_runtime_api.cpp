#include "abacus/core/target_wrapper.h"
#include "abacus/core/common.h"

namespace WW{
namespace abacus{

#ifdef USE_HIP
/**
 * targetwrapper implementation for nv host
*/
typedef TargetWrapper<AMDHIPHX86> AMDHIPH_API;

void AMDHIPH_API::getDeviceCount(int& count){
    count = 1;
}

void AMDHIPH_API::setDevice(int id){}

void AMDHIPH_API::memAlloc(void** ptr, size_t n){
    HIP_CHECK(hipHostMalloc(ptr, n));
}

void AMDHIPH_API::memFree(void* ptr){
    if(nullptr != ptr){
        HIP_CHECK(hipHostFree(ptr));
    }
}

void AMDHIPH_API::memSet(void* ptr, int value, size_t n){
    memset(ptr, value, n);
}

void AMDHIPH_API::createEvent(event_t* event, bool flag){
    if(!flag){
        HIP_CHECK(hipEventCreateWithFlags(event, hipEventDefault));
    }
    else{
        HIP_CHECK(hipEventCreateWithFlags(event, hipEventDisableTiming));
    }
}

void AMDHIPH_API::destoryEvent(event_t event){
    HIP_CHECK(hipEventDestroy(event));
}

void AMDHIPH_API::recordEvent(event_t event, stream_t stream){
    HIP_CHECK(hipEventRecord(event, stream));
}


void AMDHIPH_API::createStream(stream_t* stream){
    HIP_CHECK(hipStreamCreate(stream));
}

void AMDHIPH_API::createStreamWithFlag(stream_t* stream, unsigned int flag){
    HIP_CHECK(hipStreamCreateWithFlags(stream, flag));
}

void AMDHIPH_API::createStreamWithPriority(stream_t* stream, unsigned int flag, int priority){
    HIP_CHECK(hipStreamCreateWithPriority(stream, flag, priority));
}

void AMDHIPH_API::destoryStream(stream_t stream){
    HIP_CHECK(hipStreamDestroy(stream));
}

void AMDHIPH_API::queryEvent(event_t event){
    HIP_CHECK(hipEventQuery(event));
}

void AMDHIPH_API::syncEvent(event_t event){
    HIP_CHECK(hipEventSynchronize(event));
}

void AMDHIPH_API::syncStream(event_t event, stream_t stream){
    //HIP_CHECK(hipStreamWaitEvent(stream, event));
}

void AMDHIPH_API::syncStream(stream_t stream){
    HIP_CHECK(hipStreamSynchronize(stream));
}

void AMDHIPH_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2H){
    HIP_CHECK(hipMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyHostToHost));
}

void AMDHIPH_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2H){
    HIP_CHECK(hipMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyHostToHost, stream));
}
void AMDHIPH_API::syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count){}

void AMDHIPH_API::asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream){}
        
int AMDHIPH_API::getDeviceId(){
    return 0;
}
void AMDHIPH_API::deviceSync(){
    HIP_CHECK(hipDeviceSynchronize());
}



/**
 * targetwrapper implementation for nv device
*/
typedef TargetWrapper<AMD_HIP> AMDHIP_API;
void AMDHIP_API::getDeviceCount(int& count){
    //HIP_CHECK(hipGetDeviceCount(count));
}

void AMDHIP_API::setDevice(int idx){
    HIP_CHECK(hipSetDevice(idx));
}

void AMDHIP_API::memAlloc(void**ptr, size_t n){
    HIP_CHECK(hipMalloc(ptr, n));
}
void  AMDHIP_API::memFree(void* ptr){
    if(nullptr != ptr){
        HIP_CHECK(hipFree(ptr));
    }
   
}

void AMDHIP_API::memSet(void* ptr, int value, size_t n){
    HIP_CHECK(hipMemset(ptr, value, n));
}

void AMDHIP_API::createEvent(event_t* event, bool flag){
    if(flag){
        HIP_CHECK(hipEventCreateWithFlags(event, hipEventDefault));    
    }
    else{
        HIP_CHECK(hipEventCreateWithFlags(event, hipEventDisableTiming));
    }
}

void AMDHIP_API::destoryEvent(event_t event){
    HIP_CHECK(hipEventDestroy(event));
}

void AMDHIP_API::recordEvent(event_t event, stream_t stream){
    HIP_CHECK(hipEventRecord(event, stream));
}
void AMDHIP_API::createStream(stream_t* stream){
    HIP_CHECK(hipStreamCreate(stream));
}

void AMDHIP_API::createStreamWithFlag(stream_t* stream, unsigned int flag){
    HIP_CHECK(hipStreamCreateWithFlags(stream, flag));
}

void AMDHIP_API::createStreamWithPriority(stream_t* stream, unsigned int flag, int priority){
    //HIP_CHECK(hipStreamWithPriority(stream, flag, priority));
}

void AMDHIP_API::destoryStream(stream_t stream){
    HIP_CHECK(hipStreamDestroy(stream));
}

void AMDHIP_API::queryEvent(event_t event){
    HIP_CHECK(hipEventQuery(event));
}

void AMDHIP_API::syncEvent(event_t event){
    HIP_CHECK(hipEventSynchronize(event));
}

void AMDHIP_API::syncStream(event_t event, stream_t stream){
    //HIP_CHECK(hipStreamWaitEvent(stream, event));
}
void AMDHIP_API::syncStream(stream_t stream){
    HIP_CHECK(hipStreamSynchronize(stream));
}

void AMDHIP_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2D){
    if(dst_id == src_id){
        HIP_CHECK(hipMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyDeviceToDevice));
    }
    else{
        HIP_CHECK(hipMemcpyPeer(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<const char*>(src) + src_offset, src_id, count));
    }
}
void AMDHIP_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2D){
    HIP_CHECK(hipMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyHostToDevice));           
}

void AMDHIP_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2H){
    HIP_CHECK(hipMemcpy(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyDeviceToHost));
}

void AMDHIP_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2D){
    if(src_id == dst_id){
        HIP_CHECK(hipMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyDeviceToDevice, stream));
    }
    else{
        HIP_CHECK(hipMemcpyPeerAsync(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<const char*>(src) + src_offset, src_id, count, stream));
    }
}

void AMDHIP_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2D){
    HIP_CHECK(hipMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyHostToDevice, stream));
}
void AMDHIP_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2H){
    HIP_CHECK(hipMemcpyAsync(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count, hipMemcpyDeviceToHost, stream));
}

void AMDHIP_API::syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count){
    HIP_CHECK(hipMemcpyPeer(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<const char*>(src) + src_offset, src_id, count));            
}
void AMDHIP_API::asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream){
    HIP_CHECK(hipMemcpyPeerAsync(static_cast<char*>(dst) + dst_offset, dst_id, static_cast<const char*>(src) + src_offset, src_id, count, stream));           
}

int AMDHIP_API::getDeviceId(){
    int device_id = 0;
    HIP_CHECK(hipGetDevice(&device_id));
    return device_id;
}

void AMDHIP_API::deviceSync(){
    HIP_CHECK(hipDeviceSynchronize());
}

double AMDHIP_API::elapseMS(event_t start, event_t end){
    float ans = 0.0f;
    HIP_CHECK(hipEventElapsedTime(&ans, start, end));
    return static_cast<double>(ans);
}

#endif //USE_HIP
}// namespace abacus
}// namespace WW