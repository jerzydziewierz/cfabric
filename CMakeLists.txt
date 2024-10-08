cmake_minimum_required(VERSION 3.25)
project(cfabric)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

include_directories(.)
include_directories(src)
include_directories(src/include)

add_executable(demo1
        src/demo1.cpp
        src/include/cfabric.hpp
        )

target_link_libraries(demo1 PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)


add_executable(demo2
        src/demo2.cpp
        src/include/cfabric.hpp
        )

target_link_libraries(demo2 PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

add_executable(demo3
        src/demo3.cpp
        src/include/cfabric.hpp
        )

target_link_libraries(demo3 PRIVATE $<$<BOOL:${MINGW}>:ws2_32>)

# Add test executable
add_executable(tests
        src/tests/test_main.cpp
        src/tests/test_cfabric.cpp
        src/tests/test_cfabric_performance.cpp
        src/include/cfabric.hpp
        )

target_link_libraries(tests PRIVATE spdlog::spdlog GTest::GTest GTest::Main $<$<BOOL:${MINGW}>:ws2_32>)

# Enable testing
enable_testing()
add_test(NAME cfabric_tests COMMAND tests)



