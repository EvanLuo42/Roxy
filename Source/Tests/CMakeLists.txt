add_executable(RoxyTests
        Main.cpp
        SmokeTest.cpp
)

target_link_libraries(RoxyTests PRIVATE Roxy::Runtime doctest::doctest)

FetchContent_GetProperties(doctest SOURCE_DIR doctest_SOURCE_DIR)
list(APPEND CMAKE_MODULE_PATH ${doctest_SOURCE_DIR}/scripts/cmake)
include(doctest)
doctest_discover_tests(RoxyTests)
