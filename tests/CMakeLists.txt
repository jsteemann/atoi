# -*- mode: CMAKE; -*-
include(CTest)
enable_testing()
find_package(Threads)
add_subdirectory(googletest/googletest REQUIRED)

add_executable(tests tests.cpp)
add_test(tests tests)
target_include_directories(tests PRIVATE ${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_include_directories(tests PUBLIC ../include)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})
