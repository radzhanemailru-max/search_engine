
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/main.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
    search_engine_tests
    test.cpp
)

target_link_libraries(
    search_engine_tests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(search_engine_tests)
