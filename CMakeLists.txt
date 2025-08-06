cmake_minimum_required(VERSION 3.25)
project(scan VERSION 1.0.0 LANGUAGES CXX)

#set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=gnu++2c")

set(target scan)

add_library(${target} INTERFACE)

target_include_directories(${target} INTERFACE include/)

set(test_target scan_tests)

add_executable(${test_target} tests/main.cpp)
target_link_libraries(${test_target} PRIVATE ${target})

find_package(GTest REQUIRED)
file(GLOB TEST_SRC_FILES "${CMAKE_SOURCE_DIR}/unit_tests/*.cpp")


add_executable(${PROJECT_NAME}_unit_tests "${TEST_SRC_FILES}")
target_link_libraries(${PROJECT_NAME}_unit_tests PRIVATE ${target} GTest::gtest GTest::gtest_main)
target_include_directories(${PROJECT_NAME}_unit_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Включаем тестирование
enable_testing()
add_test(NAME scan_unit_tests COMMAND ${PROJECT_NAME}_unit_tests)

