cmake_minimum_required(VERSION 3.22.1)

project("haydra")

add_library(
        haydra
        SHARED
        haydra_native.cpp
)

add_library(llama SHARED IMPORTED)

set_target_properties(llama PROPERTIES
        IMPORTED_LOCATION
        "${CMAKE_CURRENT_LIST_DIR}/../jniLibs/${ANDROID_ABI}/libllama.so.0"
)

target_link_libraries(
        haydra
        llama
        log
        dl
)
