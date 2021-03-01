enable_testing()
include(FetchContent)
FetchContent_Declare(
        ut
        GIT_REPOSITORY https://github.com/boost-ext/ut.git
        GIT_TAG        v1.1.8
)
FetchContent_MakeAvailable(ut)