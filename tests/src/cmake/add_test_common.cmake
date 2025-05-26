function(add_test_common file_name)
    add_executable(${PROJECT_NAME}_${file_name}
            ${file_name}.cpp)
    target_link_libraries(${PROJECT_NAME}_${file_name}
            PRIVATE Boost::ut
            PRIVATE ${PROJECT_NAME}_tl
            PRIVATE ${PROJECT_NAME}_project_warnings
            PRIVATE ${PROJECT_NAME}_clang_compat
            )
    target_include_directories(${PROJECT_NAME}_${file_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    set_target_properties(${PROJECT_NAME}_${file_name} PROPERTIES OUTPUT_NAME "${file_name}")
    if (MSVC)
        target_compile_definitions(${PROJECT_NAME}_${file_name} PRIVATE _CRT_SECURE_NO_WARNINGS)
    endif()
    add_test(
            NAME ${PROJECT_NAME}_${file_name} COMMAND ${PROJECT_NAME}_${file_name}
    )
endfunction()