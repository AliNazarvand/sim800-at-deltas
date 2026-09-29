cmake_minimum_required(VERSION 3.16)
project(simcom_sim800_series LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only interface target
add_library(simcom INTERFACE)
target_include_directories(simcom INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/simcom
)

# Host-side sanity executable
add_executable(simcom_demo src/main.cpp)
target_link_libraries(simcom_demo PRIVATE simcom)

# Host-side unit test
enable_testing()
add_executable(test_database test/test_database.cpp)
target_link_libraries(test_database PRIVATE simcom)
add_test(NAME database COMMAND test_database)
