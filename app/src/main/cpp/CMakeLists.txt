cmake_minimum_required(VERSION 3.22.1)

project("haydra")

add_library(
        haydra
        SHARED
        haydra_native.cpp
)
