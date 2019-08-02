#ifndef ABACUS_CORE_ENV_HPP
#define ABACUS_CORE_ENV_HPP

#include "abacus/core/device.h"
#include "utils/common.h"

namespace WW{
namespace abacus{

template<typename TargetType>
class Env{
public:
    typedef TargetWrapper<TargetType> API;
    typedef std::vector<Device<TargetType>> Devs;

    static Devs& getEnv(){
        static Devs ins = Devs();
        return ins;
    }

    static void envInit(int max_stream = 4){
        Devs& devs = getEnv();
        if(!devs.empty()) return;
        int device_count = 0;
        API::getDeviceCount(device_count);
        if(0 == device_count){
            CHECK(false) << "No device found!";
        }
        else{
            LOG(INFO) << "Found " << device_count << " device(s)";
        }

        for(int i = 0; i < device_count; ++i){
            devs.emplace_back(Device<TargetType>(max_stream));
        }
        
        // todo: support multi GPU, Currently support single GPU.
        int cur_device_id = API::getDeviceId();
        devs[cur_device_id].createStream();
        LOG(INFO) << "Device size = " << devs.size() << ", current device idx is " << cur_device_id;
    }

    static void envExit(){}

private:
    Env() = delete;
    ~Env() = delete;
    DISABLE_COPY_ASSIGN_MOVE(Env);
};

} // namespace abacus
} // namespace WW

#endif