#ifndef ABACUS_CORE_TARGET_WRAPPER_H
#define ABACUS_CORE_TARGET_WRAPPER_H

#include "abacus/core/data_traits.h"
#include "abacus/core/target_traits.h"
#include "abacus/core/common.h"
#include "ww_config.h"

namespace WW{
namespace abacus{
template<typename TargetType, typename TargetCategory = typename TargetTraits<TargetType>::TargetCategory>
class TargetWrapper{
public:
    typedef void* event_t;
    typedef void* stream_t;

    static void getDeviceCount(int&)                                   {LOG(ERROR) << "Unimplemented yet!";}
    static void setDevice(int)                                         {LOG(ERROR) << "Unimplemented yet!";}
    static void memAlloc(void**, size_t)                               {LOG(ERROR) << "Unimplemented yet!";}
    static void memFree(void*)                                         {LOG(ERROR) << "Unimplemented yet!";}
    static void memSet(void*, int, size_t)                             {LOG(ERROR) << "Unimplemented yet!";}
    static void createEvent(event_t*, bool flag = false)               {LOG(ERROR) << "Unimplemented yet!";}
    static void destoryEvent(event_t)                                  {LOG(ERROR) << "Unimplemented yet!";}
    static void recordEvent(event_t, stream_t)                         {LOG(ERROR) << "Unimplemented yet!";}
    static void createStream(stream_t*)                                {LOG(ERROR) << "Unimplemented yet!";}
    static void createStreamWithFlag(stream_t*, unsigned int)          {LOG(ERROR) << "Unimplemented yet!";}
    static void createStreamWithPriority(stream_t*, unsigned int, int) {LOG(ERROR) << "Unimplemented yet!";}
    static void destoryStream(stream_t)                                {LOG(ERROR) << "Unimplemented yet!";}
    static void queryEvent(event_t)                                    {LOG(ERROR) << "Unimplemented yet!";}
    static void syncEvent(event_t)                                     {LOG(ERROR) << "Unimplemented yet!";}
    static void syncStream(event_t, stream_t)                          {LOG(ERROR) << "Unimplemented yet!";}
    static void syncStream(stream_t)                                   {LOG(ERROR) << "Unimplemented yet!";}
    //**************
    //memcpy
    //**************
    static int getDeviceId()                                           {LOG(ERROR) << "Unimplemented yet!"; return 0;}
    static void deviceSync()                                           {LOG(ERROR) << "Unimplemented yet!";}
};


#ifdef USE_X86
template<>
class TargetWrapper<X86>{
public:
    typedef void* event_t;
    typedef void* stream_t;

    static void getDeviceCount(int&);
    static void setDevice(int);
    static void memAlloc(void**, size_t);
    static void memFree(void*);
    static void memSet(void*, int, size_t);
    static void createEvent(event_t*, bool flag = false);
    static void destoryEvent(event_t);
    static void recordEvent(event_t, stream_t);
    static void createStream(stream_t*);
    static void createStreamWithFlag(stream_t*, unsigned int);
    static void createStreamWithPriority(stream_t*, unsigned int, int);
    static void destoryStream(stream_t);
    static void queryEvent(event_t);
    static void syncEvent(event_t);
    static void syncStream(event_t, stream_t);
    static void syncStream(stream_t);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2H);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2H);
    static void syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count);
    static void asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream);
    static int getDeviceId();
    static void deviceSync();
};
#endif

#ifdef USE_CUDA
template<>
class TargetWrapper<NVHX86>{
public:
    typedef cudaEvent_t event_t;
    typedef cudaStream_t stream_t;

    static void getDeviceCount(int&);
    static void setDevice(int);
    static void memAlloc(void**, size_t);
    static void memFree(void*);
    static void memSet(void*, int, size_t);
    static void createEvent(event_t*, bool flag = false);
    static void destoryEvent(event_t);
    static void recordEvent(event_t, stream_t);
    static void createStream(stream_t*);
    static void createStreamWithFlag(stream_t*, unsigned int);
    static void createStreamWithPriority(stream_t*, unsigned int, int);
    static void destoryStream(stream_t);
    static void queryEvent(event_t);
    static void syncEvent(event_t);
    static void syncStream(event_t, stream_t);
    static void syncStream(stream_t);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2H);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2H);
    static void syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count);
    static void asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream);
    static int getDeviceId();
    static void deviceSync();
};

template<>
class TargetWrapper<NV>{
public:
    typedef cudaEvent_t event_t;
    typedef cudaStream_t stream_t;

    static void getDeviceCount(int&);
    static void setDevice(int);
    static void memAlloc(void**, size_t);
    static void memFree(void*);
    static void memSet(void*, int, size_t);
    static void createEvent(event_t*, bool flag = false);
    static void destoryEvent(event_t);
    static void recordEvent(event_t, stream_t);
    static void createStream(stream_t*);
    static void createStreamWithFlag(stream_t*, unsigned int);
    static void createStreamWithPriority(stream_t*, unsigned int, int);
    static void destoryStream(stream_t);
    static void queryEvent(event_t);
    static void syncEvent(event_t);
    static void syncStream(event_t, stream_t);
    static void syncStream(stream_t);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2D);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2D);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2H);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2D);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2D);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2H);
    static void syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count);
    static void asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream);
    static int getDeviceId();
    static void deviceSync();

    static double elapseMS(event_t start, event_t end);
};
#endif //USE_CUDA

#ifdef USE_HIP
template<>
class TargetWrapper<AMDHIPHX86>{
public:
    typedef hipEvent_t event_t;
    typedef hipStream_t stream_t;

    static void getDeviceCount(int&);
    static void setDevice(int);
    static void memAlloc(void**, size_t);
    static void memFree(void*);
    static void memSet(void*, int, size_t);
    static void createEvent(event_t*, bool flag = false);
    static void destoryEvent(event_t);
    static void recordEvent(event_t, stream_t);
    static void createStream(stream_t*);
    static void createStreamWithFlag(stream_t*, unsigned int);
    static void createStreamWithPriority(stream_t*, unsigned int, int);
    static void destoryStream(stream_t);
    static void queryEvent(event_t);
    static void syncEvent(event_t);
    static void syncStream(event_t, stream_t);
    static void syncStream(stream_t);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2H);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2H);
    static void syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count);
    static void asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream);
    static int getDeviceId();
    static void deviceSync();
};

template<>
class TargetWrapper<AMD_HIP>{
public:
    typedef hipEvent_t event_t;
    typedef hipStream_t stream_t;

    static void getDeviceCount(int&);
    static void setDevice(int);
    static void memAlloc(void**, size_t);
    static void memFree(void*);
    static void memSet(void*, int, size_t);
    static void createEvent(event_t*, bool flag = false);
    static void destoryEvent(event_t);
    static void recordEvent(event_t, stream_t);
    static void createStream(stream_t*);
    static void createStreamWithFlag(stream_t*, unsigned int);
    static void createStreamWithPriority(stream_t*, unsigned int, int);
    static void destoryStream(stream_t);
    static void queryEvent(event_t);
    static void syncEvent(event_t);
    static void syncStream(event_t, stream_t);
    static void syncStream(stream_t);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2D);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2D);
    static void syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __D2H);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2D);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2D);
    static void asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __D2H);
    static void syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count);
    static void asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream);
    static int getDeviceId();
    static void deviceSync();

    static double elapseMS(event_t start, event_t end);
};
#endif //USE_HIP


}// namespace abacus
}// namespace WW
#endif