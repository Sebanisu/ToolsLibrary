add_executable(${PROJECT_NAME}_input_test
        input_test.cpp)
target_link_libraries(${PROJECT_NAME}_input_test PRIVATE Boost::ut ${PROJECT_NAME}_tl)
set_target_properties(${PROJECT_NAME}_input_test PROPERTIES OUTPUT_NAME "input_test")
add_test(
        NAME ${PROJECT_NAME}_INPUT_TEST COMMAND ${PROJECT_NAME}_input_test
)
add_executable(${PROJECT_NAME}_read_test
        read_test.cpp)
target_link_libraries(${PROJECT_NAME}_read_test PRIVATE Boost::ut ${PROJECT_NAME}_tl)
set_target_properties(${PROJECT_NAME}_read_test PROPERTIES OUTPUT_NAME "read_test")
add_test(
        NAME ${PROJECT_NAME}_READ_TEST COMMAND ${PROJECT_NAME}_read_test
)
