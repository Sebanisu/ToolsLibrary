include(cmake/compiler_warnings.cmake)
add_library(${PROJECT_NAME}_project_warnings INTERFACE)
set_project_warnings(${PROJECT_NAME}_project_warnings ON)
add_executable(${PROJECT_NAME}_input_test
        input_test.cpp)
target_link_libraries(${PROJECT_NAME}_input_test
        PRIVATE Boost::ut
        PRIVATE ${PROJECT_NAME}_tl
        PRIVATE ${PROJECT_NAME}_project_warnings)
set_target_properties(${PROJECT_NAME}_input_test PROPERTIES OUTPUT_NAME "input_test")
add_test(
        NAME ${PROJECT_NAME}_INPUT_TEST COMMAND ${PROJECT_NAME}_input_test
)
add_executable(${PROJECT_NAME}_read_test
        read_test.cpp)
target_link_libraries(${PROJECT_NAME}_read_test
        PRIVATE Boost::ut
        PRIVATE ${PROJECT_NAME}_tl
        PRIVATE ${PROJECT_NAME}_project_warnings)
set_target_properties(${PROJECT_NAME}_read_test PROPERTIES OUTPUT_NAME "read_test")
add_test(
        NAME ${PROJECT_NAME}_READ_TEST COMMAND ${PROJECT_NAME}_read_test
)
