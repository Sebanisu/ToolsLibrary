add_executable(${PROJECT_NAME}_ut_1
        ut_1.cpp)
target_link_libraries(${PROJECT_NAME}_ut_1 PRIVATE Boost::ut ${PROJECT_NAME}_tl)
set_target_properties(${PROJECT_NAME}_ut_1 PROPERTIES OUTPUT_NAME "ut_1")
add_test(
        NAME ${PROJECT_NAME}_UT_1 COMMAND ${PROJECT_NAME}_ut_1
)
