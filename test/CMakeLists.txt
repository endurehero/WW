
wwFetchFilesWithSuffix(${WW_TEST}/abacus "cpp" WW_TEST_CASE_SRC)

install(FILES ${PROJECT_BINARY_DIR}/ww_config.h
        DESTINATION ${PROJECT_SOURCE_DIR}/${WW_OUTPUT_PATH}/include)

#build test cases
foreach(src_name ${WW_TEST_CASE_SRC})
    #get filename without suffix
    string(REPLACE "/" ";" test_case_name_list ${src_name})
    list(GET test_case_name_list -1 test_case_name)
    string(REPLACE "." ";" test_case_name_no_subfix_list ${test_case_name})
    list(GET test_case_name_no_subfix_list 0 test_name)
    
    add_executable(${test_name} ${src_name})

    if(BUILD_SHARED)
        target_link_libraries(${test_name} ${WW_ABACUS_LIB_TARGET})
    else() #build shared
        target_link_libraries(${test_name} -Wl,--whole-archive ${WW_ABACUS_LIB_TARGET} -Wl,--no-whole-archive)
    endif()

    set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                          ${PROJECT_SOURCE_DIR}/${WW_OUTPUT_PATH}/unit_test)
endforeach()