#ifndef UTILS_UNIT_TEST_TEST_BASE_H
#define UTILS_UNIT_TEST_TEST_BASE_H

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
    
protected:
    virtual void setup() = 0;
    virtual void teardown() = 0;
};

TestBase::~TestBase(){}
}// namespace test

}// namespace WW

#endif