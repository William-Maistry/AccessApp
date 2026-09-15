cmake_minimum_required(VERSION 3.22)

project(OpenScanSA)

add_subdirectory(core)

add_library(
        native-scanner
        SHARED
        NativeScanner.cpp
        scanner.cpp
)

add_library(
        SWIDecoder
        SHARED
        SWIDecoder.cpp
)

target_include_directories(
        native-scanner
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core/core/src
)

target_link_libraries(
        native-scanner
        ZXing
        log
)

target_link_libraries(
        SWIDecoder
        log
)