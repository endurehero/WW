#ifndef ABACUS_ABACUS_TYPE_H
#define ABACUS_ABACUS_TYPE_H

#include <memory>
#include <iostream>
#include "utils/common.h"
#include "utils/log/logger.h"

namespace WW{
namespace abacus{

struct __INVALID_TYPE{};

// target category
struct __HOST_TARGET{};
struct __DEVICE_TARGET{};
// target type
struct __X86_DEVICE{};
struct __CUDA_DEVICE{};
struct __HIP_DEVICE{};
struct __OCL_DEVICE{};

// memory copy directions
struct __H2D{};
struct __H2H{};
struct __D2H{};
struct __D2D{};

typedef enum{
    LAYOUT_INVALID = 0,
    LAYOUT_NCHW = 1
}LAYOUT_TYPE;

inline std::string layoutStr(LAYOUT_TYPE type){
    switch(type){
        case LAYOUT_NCHW:
            return "NCHW Layout";
        default:
            return "Invalid Layout";
    }
}

class Layout{
public:
    virtual int batchIndex()   const { return -1; }
    virtual int channelIndex() const { return -1; }
    virtual int heightIndex()  const { return -1; }
    virtual int widthIndex()   const { return -1; }
    virtual int dims()         const { return -1; }
    virtual LAYOUT_TYPE type()         const { return LAYOUT_INVALID; }
};

class NCHW : public Layout{
public:
    int batchIndex()   const { return 0; }
    int channelIndex() const { return 1; }
    int heightIndex()  const { return 2; }
    int widthIndex()   const { return 3; }
    int dims()         const { return 4; }
    LAYOUT_TYPE type() const { return LAYOUT_NCHW; }
};

class LayoutSimpleFactory{
public:
    static std::shared_ptr<Layout> create(LAYOUT_TYPE type){
        std::shared_ptr<Layout> ans;
        switch(type){
            case LAYOUT_INVALID:
                LOG(FATAL) << "The layout_type is invalid.";
                break;
            case LAYOUT_NCHW:
                ans = std::make_shared<NCHW>();
                break;
            default:
                LOG(FATAL) << "The layout_type is invalid.";
                break;
        }
        
        return ans;
    }

private:
    LayoutSimpleFactory() = delete;
    ~LayoutSimpleFactory() = delete;
    DISABLE_COPY_ASSIGN_MOVE(LayoutSimpleFactory);
};

typedef enum{
    DATA_INVALID  = -1,
    DATA_VOID     = 0,
    DATA_HALF     = 1,
    DATA_FLOAT    = 2,
    DATA_DOUBLE   = 3,
    DATA_INT8     = 4,
    DATA_INT16    = 5,
    DATA_INT32    = 6,
    DATA_INT64    = 7,
    DATA_UINT8    = 8,
    DATA_UINT16   = 9,
    DATA_UINT32   = 10,
    DATA_UINT64   = 11,
    DATA_STRING   = 12,
    DATA_BOOL     = 13,
    DATA_SHAPE    = 14,
    DATA_TENSOR   = 15
}DATA_TYPE;


typedef enum{
    AbacusSuccess        = 0,
    AbacusNotInitialized = 1,
    AbacusInvalidValue   = 2,
    AbacusMemAllocFailed = 3,
    AbacusUnknowError    = 4,
    AbacusOutofAuthority = 5,
    AbacusOutofMem       = 6,
    AbacusUnImplError    = 7,
    AbacusWrongDevice    = 8
}ABACUS_STATUS;


typedef enum {
    TARGET_INVALID = 0,
    TARGET_CUDA    = 1,
    TARGET_HIP     = 2,
    TARGET_OCL     = 3,
    TARGET_HX86    = 4,
    TARGET_HCUDA   = 5,
    TARGET_HHIP    = 6,
    TARGET_HOCL    = 7
}TARGET_TYPE;

template <TARGET_TYPE type>
struct __TargetType{};
// device without pinned memory
typedef __TargetType<TARGET_CUDA> NV;
typedef __TargetType<TARGET_HIP> AMD_HIP;
typedef __TargetType<TARGET_OCL> AMD_OCL;
// Host memory
typedef __TargetType<TARGET_HX86> X86;
// device with pinned memory
typedef __TargetType<TARGET_HCUDA> NVHX86;
typedef __TargetType<TARGET_HOCL> AMDOCLHX86;
typedef __TargetType<TARGET_HHIP> AMDHIPHX86;
// invalid target
typedef __TargetType<TARGET_INVALID> INVALID;


typedef enum{
    RANDOM = 0,
    SPECIAL
}TEST_DATA_TYPE;

} // namespace abacus
} // namespace WW

#endif