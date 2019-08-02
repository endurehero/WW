#include "abacus/core/target_wrapper.h"

namespace WW{
namespace abacus{


static const int MALLOC_ALIGN = 64;


static inline void* mallocAlign(size_t size){
    size_t offset = sizeof(void*) + MALLOC_ALIGN - 1;
    char* p = static_cast<char*>(malloc(size + offset));
    
    if(!p){
        return nullptr;
    }

    void* r = reinterpret_cast<void*>(reinterpret_cast<size_t>(p + offset) & (~(MALLOC_ALIGN - 1)));
    static_cast<void**>(r)[-1] = p;
    memset(r, 0, size);
    return r;
}

static inline void freeAlign(void* ptr){
    if(ptr){
        void* p = static_cast<void**>(ptr)[-1];
        free(p); p = nullptr;
    }
}

typedef TargetWrapper<X86> X86_API;


void X86_API::getDeviceCount(int& count){
    count = 1;
}

void X86_API::setDevice(int){}

void X86_API::memAlloc(void** ptr, size_t n){
    *ptr = static_cast<void*>(mallocAlign(n));
}

void X86_API::memFree(void* ptr){
    freeAlign(ptr);
}

void X86_API::memSet(void* ptr, int value, size_t n){
    memset(ptr, value, n);
}

void X86_API::createEvent(event_t* event, bool flag){}

void X86_API::destoryEvent(event_t event){}

void X86_API::recordEvent(event_t event, stream_t stream){}

void X86_API::createStream(stream_t* stream){}

void X86_API::createStreamWithFlag(stream_t* stream, unsigned int flag) {}
void X86_API::createStreamWithPriority(stream_t* stream, unsigned int flag, int priority){}
void X86_API::destoryStream(stream_t stream){}
void X86_API::queryEvent(event_t event){}
void X86_API::syncEvent(event_t event){}
void X86_API::syncStream(event_t event, stream_t stream){}
void X86_API::syncStream(stream_t stream){}
void X86_API::syncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, __H2H){
    memcpy(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count);            
}
void X86_API::asyncMemcpy(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream, __H2H){
    memcpy(static_cast<char*>(dst) + dst_offset, static_cast<const char*>(src) + src_offset, count);
}

void X86_API::syncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, size_t count){}


void X86_API::asyncMemcpyP2p(void* dst, size_t dst_offset, int dst_id, \
        const void* src, size_t src_offset, int src_id, \
        size_t count, stream_t stream){}

int X86_API::getDeviceId(){
    return 0;
}

void X86_API::deviceSync(){}

}// namespace abacus
}// namespace WW