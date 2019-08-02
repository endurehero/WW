#ifndef UTILS_COMMON_H
#define UTILS_COMMON_H


#include <stdlib.h>
#include <string>

#define DISABLE_COPY_ASSIGN_MOVE(classname)           \
    classname(const classname&) = delete;             \
    classname(const classname&&) = delete;            \
    classname& operator= (const classname&) = delete; \
    classname& operator= (const classname&&) = delete

namespace WW{
namespace config{
    static bool isColorTerminal = false;
    inline void initial(){
        std::string term(getenv("TERM"));
        
        if(!term.empty()){
            
            if("cygwin"         == term ||
               "screen"         == term ||
               "xterm"          == term ||
               "xterm-256color" == term ||
               "xterm-color"    == term){

                isColorTerminal = true;
            }
        }

    }

    inline const char* reset()     { return isColorTerminal ? "\e[0m" : "";}
    inline const char* bold()      { return isColorTerminal ? "\e[1m" : "";}
    inline const char* dim()       { return isColorTerminal ? "\e[2m" : "";}
    inline const char* underline() { return isColorTerminal ? "\e[4m" : "";}
    inline const char* blink()     { return isColorTerminal ? "\e[5m" : "";}

    inline const char* red()    { return isColorTerminal ? "\e[31m" : "";}
    inline const char* green()  { return isColorTerminal ? "\e[32m" : "";}
    inline const char* yellow() { return isColorTerminal ? "\e[33m" : "";}
    inline const char* blue()   { return isColorTerminal ? "\e[34m" : "";}
    inline const char* purple() { return isColorTerminal ? "\e[35m" : "";}
    inline const char* cyan()   { return isColorTerminal ? "\e[36m" : "";}
    inline const char* white()  { return isColorTerminal ? "\e[37m" : "";}
    

} //namespace config
} //namespace WW
#endif