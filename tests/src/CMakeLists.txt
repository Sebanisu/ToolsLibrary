add_executable(${PROJECT_NAME}_ut_1
        ut_1.cpp)
target_link_libraries(${PROJECT_NAME}_ut_1 PRIVATE Boost::ut ${PROJECT_NAME}_tl)
