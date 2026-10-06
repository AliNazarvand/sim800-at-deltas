cmake_minimum_required(VERSION 3.16)
project(simcom_sim800_series VERSION 1.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

option(SIMCOM_ENABLE_ASAN     "Enable AddressSanitizer"                OFF)
option(SIMCOM_ENABLE_UBSAN    "Enable UndefinedBehaviorSanitizer"      OFF)
option(SIMCOM_ENABLE_COVERAGE "Enable coverage reporting"              OFF)
option(SIMCOM_ENABLE_FUZZING  "Enable libFuzzer targets (Clang only)"  OFF)

add_library(simcom INTERFACE)
target_include_directories(simcom INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(DIRECTORY include/simcom DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS simcom EXPORT simcomTargets)
install(EXPORT simcomTargets
    FILE simcomTargets.cmake
    NAMESPACE simcom::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simcom)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/simcom-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/simcom-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simcom)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/simcom-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/simcom-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/simcom-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/simcom)

configure_file(simcom.pc.in simcom.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/simcom.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE
    DESTINATION share/doc/simcom_sim800_series)

add_executable(simcom_demo src/main.cpp)
target_link_libraries(simcom_demo PRIVATE simcom)

enable_testing()
add_executable(test_database     test/test_database.cpp)
add_executable(test_api          test/test_api.cpp)
add_executable(test_compile_time test/test_compile_time.cpp)
add_executable(test_roundtrip    test/test_roundtrip.cpp)
add_executable(test_golden       test/test_golden.cpp)
add_executable(test_api_property test/test_api_property.cpp)

foreach(t test_database test_api test_compile_time test_roundtrip test_golden test_api_property)
    target_link_libraries(${t} PRIVATE simcom)
endforeach()

target_include_directories(test_roundtrip PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test)

add_test(NAME database     COMMAND test_database)
add_test(NAME api          COMMAND test_api)
add_test(NAME compile_time COMMAND test_compile_time)
add_test(NAME roundtrip    COMMAND test_roundtrip)
add_test(NAME golden       COMMAND test_golden)
add_test(NAME api_property COMMAND test_api_property)

find_package(Python3 COMPONENTS Interpreter REQUIRED)

add_test(NAME exporters
    COMMAND ${Python3_EXECUTABLE} -m pytest test/test_exporters.py)
add_test(NAME property
    COMMAND ${Python3_EXECUTABLE} -m pytest test/test_property.py)

add_custom_target(simcom_generate
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/generate_cpp.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(simcom_validate
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/validate.py
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/validate_yaml.py --strict
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_target(simcom_export
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/export_json.py
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/export_csv.py
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/export_markdown.py
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/tools/export_html.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# Fuzzing target (Clang only)
if(SIMCOM_ENABLE_FUZZING)
    if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        message(FATAL_ERROR "SIMCOM_ENABLE_FUZZING requires Clang")
    endif()
    add_executable(test_fuzz_json test/test_fuzz_json.cpp)
    target_link_libraries(test_fuzz_json PRIVATE simcom)
    target_compile_options(test_fuzz_json PRIVATE
        -fsanitize=fuzzer,address,undefined -g -O1)
    target_link_options(test_fuzz_json PRIVATE
        -fsanitize=fuzzer,address,undefined)
endif()

# Sanitizers
if(SIMCOM_ENABLE_ASAN OR SIMCOM_ENABLE_UBSAN)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address,undefined)
endif()

if(SIMCOM_ENABLE_COVERAGE)
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()
# ---------------------------------------------------------------------------
# Patch by fix script: WORKING_DIRECTORY for tests that open files
# via relative paths (golden reads test/golden/README.md and expected_outputs.txt).
# ---------------------------------------------------------------------------
if(TARGET test_golden)
    set_property(TEST golden PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
if(TARGET test_roundtrip)
    set_property(TEST roundtrip PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
if(TARGET test_api)
    set_property(TEST api PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
endif()
# Python tests need source root as CWD because they reference paths like
# test/test_exporters.py and tools/*.py relatively.
set_property(TEST exporters PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set_property(TEST property  PROPERTY WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")