#ifndef UTILS_UNIT_TEST_TEST_ENGINE_H
#define UTILS_UNIT_TEST_TEST_ENGINE_H

#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <stdio.h>

#include "test_base.h"
#include "../common.h"

namespace WW{

namespace test{


class EngineRepo{
public:
    static EngineRepo& getInstance(){
        static EngineRepo ins = EngineRepo();
        return ins;
    }
    
    ~EngineRepo(){}
    
    friend class EngineRepoOp;
    friend class EngineTest;
private:
    std::unordered_map<TestBase*, std::vector<std::function<void(void)>>> _map_class2func;
    std::unordered_map<std::string, std::vector<std::string>> _map_classname2funcname;
    std::unordered_map<std::string, TestBase*> _map_classname2class;

    EngineRepo(){};
    DISABLE_COPY_ASSIGN_MOVE(EngineRepo);
};

class EngineRepoOp{
public:
    EngineRepoOp(const char* classname, const char* funcname)
        :_class_name(classname), _func_name(funcname){
        EngineRepo& engine = EngineRepo::getInstance();
        engine._map_classname2funcname[_class_name].emplace_back(_func_name);
    }
    
    ~EngineRepoOp(){}

    EngineRepoOp& operator<<(const TestBase& test_class){
        EngineRepo& engine = EngineRepo::getInstance();
        engine._map_classname2class[_class_name] = &(const_cast<TestBase&>(test_class));
        _class = engine._map_classname2class[_class_name];
        return *this;
    }

    EngineRepoOp& operator & (const std::function<void(void)>& test_func){
        EngineRepo& engine = EngineRepo::getInstance();
        engine._map_class2func[_class].emplace_back(test_func);
    }
    
private:
    std::string _class_name;
    std::string _func_name;
    TestBase* _class;
    
};

class EngineTest{
public:

    static EngineTest& getInstance(){
        static EngineTest ins = EngineTest();
        return ins;
    }

    bool run(const char* app_name){
        fprintf(stderr, "%s%s%s[******************]%s %sRunning main() for %s%s.\n",
                ::WW::config::reset(), ::WW::config::bold(), ::WW::config::green(), ::WW::config::reset(),
                ::WW::config::dim(), app_name, ::WW::config::reset());
        
        EngineRepo& engine = EngineRepo::getInstance();
        int test_case_num = 0, test_func_num = 0;
        for(auto& item : engine._map_class2func){
            ++test_case_num;
            test_func_num += item.second.size();
        }

        fprintf(stderr, "%s%s%s[    SUM    ]%s %sRunning%s %s%d%s %stest function from%s %s%d%s %stest class%s.\n\n",
                ::WW::config::reset(),
                ::WW::config::bold(), ::WW::config::green(), ::WW::config::reset(),
                ::WW::config::dim(), ::WW::config::reset(),
                ::WW::config::white(), test_func_num, ::WW::config::reset(),
                ::WW::config::dim(), ::WW::config::reset(),
                ::WW::config::white(), test_case_num, ::WW::config::reset(),
                ::WW::config::dim(), ::WW::config::reset());
        
        if(0 == test_case_num || 0 == test_func_num){
            return false;
        }
        
        for(auto& item : engine._map_classname2funcname){
            std::string class_name = item.first;
            std::vector<std::string>& func_name_list = item.second;
            fprintf(stderr,"%s%s%s[===========]%s Running %d tests from %s.\n",
                    ::WW::config::reset(),
                    ::WW::config::bold(), ::WW::config::green(), ::WW::config::reset(),
                    static_cast<int>(func_name_list.size()),class_name.c_str());

            
            double elapsed = 0.0;
            for(size_t i = 0; i < func_name_list.size(); ++i){
                fprintf(stderr,"%s%s[ RUN       ]%s %s.%s\n",
                        ::WW::config::reset(),
                        ::WW::config::green(),::WW::config::reset(),
                        class_name.c_str(),func_name_list[i].c_str());
                
                HostTimer timer;
                timer.start();
                std::function<void(void)> test_func = engine._map_class2func[engine._map_classname2class[class_name]][i];
                test_func();
                timer.end();
                elapsed += timer.elapsed();
            }

            fprintf(stderr,"%s%s%s[===========]%s %d tests from %s class ran. ( %s%0.2lf ms%s total )\n\n",
                    ::WW::config::reset(),
                    ::WW::config::bold(),::WW::config::green(),::WW::config::reset(),
                    static_cast<int>(func_name_list.size()),class_name.c_str(),
                    ::WW::config::red(),elapsed,::WW::config::reset());
            
        }
        return true;
    }
    ~EngineTest(){}
private:
    EngineTest(){}
    DISABLE_COPY_ASSIGN_MOVE(EngineTest);
};


}// namespace test
}// namespace WW

#endif