#ifndef UTILS_TIMER_HPP
#define UTILS_TIMER_HPP

#include <chrono>
#include <list>
#include <memory>

#include "abacus/core/context.hpp"
#include "abacus/core/common.h"
#include "abacus/core/target_wrapper.h"

namespace WW{

typedef enum{
    NANO_SECONDS  = 0,
    MICRO_SECONDS = 1,
    MILLI_SECONDS = 2,
    SECONDS       = 3,
    MINUTES       = 4,
    HOURS         = 5,
}TIME_PRECISION;

template <typename TargetType>
class Timer final{
public:
    Timer(const abacus::Context<TargetType>& ctx)
        :_precision(MILLI_SECONDS){}
    
    explicit Timer(TIME_PRECISION p)
        :_precision(p){}

    ~Timer(){}

    void start(){
        _start = std::chrono::system_clock::now();
    }
    
    void end(){
        _end = std::chrono::system_clock::now();
        _time_intervals.push_back(elapsed());
    }

    double elapsed() const{

        switch(_precision){
            case NANO_SECONDS:
                return std::chrono::duration_cast<std::chrono::nanoseconds>(_end - _start).count();
            case MICRO_SECONDS:
                return std::chrono::duration_cast<std::chrono::microseconds>(_end - _start).count();
            case MILLI_SECONDS:
                return std::chrono::duration_cast<std::chrono::milliseconds>(_end - _start).count();
            case SECONDS:
                return std::chrono::duration_cast<std::chrono::seconds>(_end - _start).count();
            case MINUTES:
                return std::chrono::duration_cast<std::chrono::minutes>(_end - _start).count();
            case HOURS:
                return std::chrono::duration_cast<std::chrono::hours>(_end - _start).count();
            default:
                return -1;
                
        }

    }

    void clear(){
        _time_intervals.clear();
    }

    const std::list<double>& getIntervals() const{
        return _time_intervals;
    }

    double getAvgElapsed() const{
        double ans = 0.0;
        if(_time_intervals.empty()) return ans;
        
        for(auto t : _time_intervals){
            ans += t;
        }

        ans /= _time_intervals.size();
        return ans;
    }

private:
    TIME_PRECISION _precision;
    std::chrono::time_point<std::chrono::system_clock> _start;
    std::chrono::time_point<std::chrono::system_clock> _end;
    std::list<double> _time_intervals;
};


#ifdef USE_CUDA
template<>
class Timer<NV> final{
public:
    typedef abacus::TargetWrapper<NV> API;
    
    explicit Timer(const abacus::Context<NV>& ctx)
        :_precision(MILLI_SECONDS),
         _ctx(std::make_shared<abacus::Context<NV>>(ctx)){

        API::createEvent(&_start);
        API::createEvent(&_end);
    }
    
    Timer(TIME_PRECISION p, const abacus::Context<NV>& ctx)
        :_precision(p),
         _ctx(std::make_shared<abacus::Context<NV>>(ctx)){
        
        API::createEvent(&_start);
        API::createEvent(&_end);  
    }


    ~Timer(){
        API::destoryEvent(_start);
        API::destoryEvent(_end);
    }

    void start(){
        typename API::stream_t stream;
        stream = _ctx->getComputeStream();
        API::recordEvent(_start, stream);
    }
    
    void end(){
        typename API::stream_t stream;
        stream = _ctx->getComputeStream();
        API::recordEvent(_end, stream);
        API::syncEvent(_end);
        _time_intervals.push_back(elapsed());
    }
    
    double elapsed(){
        return API::elapseMS(_start, _end);
    }

    void clear(){
        _time_intervals.clear();
    }

    const std::list<double>& getIntervals() const{
        return _time_intervals;
    }

    double getAvgElapsed() const{
        double ans = 0.0;
        if(_time_intervals.empty()) return ans;
        
        for(auto t : _time_intervals){
            ans += t;
        }

        ans /= _time_intervals.size();
        return ans;
    }


    ~Timer(){}

private:
    TIME_PRECISION _precision;
    typename API::event_t _start;
    typename API::event_t _end;
    std::list<double> _time_intervals;
    std::shared_ptr<abacus::Context<NV>> _ctx;

};
#endif // USE_CUDA

} // namespace WW
#endif