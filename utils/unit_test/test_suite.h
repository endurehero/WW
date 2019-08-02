#ifndef UTILS_UNIT_TEST_TEST_SUITE_H
#define UTILS_UNIT_TEST_TEST_SUITE_H

#include "test_engine.h"
#include "test_base.h"

#define DECONSTRUCT_PREFIX ~
#define TEST(test_case, test_func) \
    class test_case##_##test_func : public test_case{ \
    public:\
        test_case##_##test_func(){}\
        ~test_case##_##test_func(){}\
        void test_function();\
    };\
    \
    const test_case##_##test_func _##test_case##_##test_func;\
    std::function<void(void)> func_##test_case##_##test_func = std::bind(&test_case##_##test_func::test_function, _##test_case##_##test_func);\
    \
    ::WW::test::EngineRepoOp op_##test_case##_##test_func = (::WW::test::EngineRepoOp(#test_case, #test_func) \
    << test_case::getInstance<test_case>() & func_##test_case##_##test_func);\
    void test_case##_##test_func::test_function()


#define InitTest()\
    ::WW::config::initial()

#define RUN_ALL_TESTS(app_name)\
    ::WW::test::EngineTest::getInstance().run(app_name)

#endif
