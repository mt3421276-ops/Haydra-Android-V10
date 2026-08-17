cmake_minimum_required(VERSION 3.22.1)

project("haydra")

add_library(
        haydra
        SHARED
        haydra_native.cpp
)

target_include_directories(
        haydra
        PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/../../../../../../llama.cpp/include"
        "${CMAKE_CURRENT_LIST_DIR}/../../../../../../llama.cpp/ggml/include"
)

add_library(llama SHARED IMPORTED)

set_target_properties(
        llama
        PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/${ANDROID_ABI}/libllama.so.0"
)

add_library(ggml SHARED IMPORTED)

set_target_properties(
        ggml
        PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/${ANDROID_ABI}/libggml.so.0"
)

add_library(ggml-cpu SHARED IMPORTED)

set_target_properties(
        ggml-cpu
        PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/${ANDROID_ABI}/libggml-cpu.so.0"
)

add_library(ggml-base SHARED IMPORTED)

set_target_properties(
        ggml-base
        PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/${ANDROID_ABI}/libggml-base.so.0"
)

target_link_libraries(
        haydra
        llama
        ggml
        ggml-cpu
        ggml-base
        log
        dl
)
