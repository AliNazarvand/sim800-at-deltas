# Integration Guide

## As a CMake subproject (FetchContent)

    include(FetchContent)
    FetchContent_Declare(simcom
        GIT_REPOSITORY https://github.com/AliNazarvand/sim800-at-deltas.git
        GIT_TAG        main)
    FetchContent_MakeAvailable(simcom)

    target_link_libraries(my_app PRIVATE simcom)

## As an installed package

    find_package(simcom REQUIRED)
    target_link_libraries(my_app PRIVATE simcom::simcom)

## Via pkg-config

    pkg-config --cflags --libs simcom

## Header include

    #include "simcom/simcom.hpp"