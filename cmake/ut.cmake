#https://stackoverflow.com/a/47675199/2588183
#function(my_add_subdirectory _dir)
#    get_filename_component(_fullpath "${_dir}" REALPATH)
#    if (EXISTS "${_fullpath}" AND EXISTS "${_fullpath}/CMakeLists.txt")
#        get_property(_included_dirs GLOBAL PROPERTY GlobalAddSubdirectoryOnceIncluded)
#        list(FIND _included_dirs "${_fullpath}" _used_index)
#        if (${_used_index} EQUAL -1)
#            set_property(GLOBAL APPEND PROPERTY GlobalAddSubdirectoryOnceIncluded "${_fullpath}")
#            add_subdirectory(${_dir} ${ARGN})
#        endif ()
#    else ()
#        message(WARNING "my_add_subdirectory: Can't find ${_fullpath}/CMakeLists.txt")
#    endif ()
#endfunction(my_add_subdirectory)
enable_testing()
include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG origin/master
)
#FetchContent_MakeAvailable(ut)
FetchContent_GetProperties(ut)
if (NOT ut_POPULATED)
    FetchContent_Populate(ut)
    if (EXISTS "${ut_SOURCE_DIR}" AND EXISTS "${ut_BINARY_DIR}")
        add_subdirectory(${ut_SOURCE_DIR} ${ut_BINARY_DIR} EXCLUDE_FROM_ALL)
    endif ()
endif ()