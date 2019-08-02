function(wwFetchIncludeRecursively root_dir)
    if(IS_DIRECTORY ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)
    foreach(sub ${ALL_SUB})
        if(IS_DIRECTORY ${root_dir}/${sub})
            wwFetchIncludeRecursively(${root_dir}/${sub})
        endif() 
    endforeach()
endfunction()

function(wwFetchFilesWithSuffix search_dir suffix outputs)
    exec_program(ls ${search_dir}
                ARGS "*.${suffix}"
                OUTPUT_VARIABLE OUTPUT
                RETURN_VALUE VALUE)
    
    if(NOT VALUE)
        string(REPLACE "\n" ";" OUTPUT_LIST "${OUTPUT}")
        set(abs_dir "")
        foreach(var ${OUTPUT_LIST})
            set(abs_dir ${abs_dir} ${search_dir}/${var})
        endforeach()

        set(${outputs} ${${outputs}} ${abs_dir} PARENT_SCOPE)
    endif()
endfunction()

macro(wwOption variable description value)
    set(_value ${value})
    set(_condition "")
    set(_varname "_value")
    
    foreach(arg ${ARGN})
        if(arg STREQUAL "IF" OR arg STREQUAL "if")
            set(_varname "_condition")
        else()
            list(APPEND ${_varname} ${arg})
        endif()
    endforeach()
    unset(_varname)

    if(_condition STREQUAL "")
        set(_condition 2 GREATER 1)
    endif()

    if(${_condition})
        option(${variable} "${description}" ${_value})
    else()
        unset(${variable} CACHE)
    endif()

    unset(_condition)
    unset(_value)
endmacro()

macro(wwCheckFlagSupport lang flag varname)
    if("_${lang}_" MATCHS "_CXX_")
        set(_lang CXX)
    elseif("_${lang}_" MATCHS "_CU_")
        set(_lang NVCC)
    else()
        set(_lang ${lang})
    endif()

    string(TOUPPER "${flag}" ${varname})

    
endmacro()

macro(wwAddCompileOption option)
    
endmacro()