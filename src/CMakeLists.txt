# Interface library is great if this is a header only library.
add_library(tl INTERFACE)

# add this directory to the include search path:
target_include_directories(tl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# use target_link_libraries(... tl) to add this library to your existing project.