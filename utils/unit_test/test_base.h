#ifndef UTILS_UNIT_TEST_TEST_BASE_H
#define UTILS_UNIT_TEST_TEST_BASE_H

#include <chrono>

namespace WW{

namespace test{

class TestBase{
public:
    TestBase(){};
    virtual ~TestBase() = 0;

    template<typename DeriveClass>
    static DeriveClass& getInstance(){
        static DeriveClass ins = DeriveClass();
        return ins;
    }
};

typedef enum{
    NANO_SECONDS  = 0,
    MICRO_SECONDS = 1,
    MILLI_SECONDS = 2,
    SECONDS       = 3,
    MINUTES       = 4,
    HOURS         = 5,
}TIME_PRECISION;

class HostTimer{
public:
    HostTimer()
        :_precision(MILLI_SECONDS){}
    
    explicit HostTimer(TIME_PRECISION p)
        :_precision(p){}

    void start(){
        _start = std::chrono::system_clock::now();
    }
    
    void end(){
        _end = std::chrono::system_clock::now();
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

private:
    TIME_PRECISION _precision;
    std::chrono::time_point<std::chrono::system_clock> _start;
    std::chrono::time_point<std::chrono::system_clock> _end;
};


}// namespace test 

}// namespace WW

#endif