cmake_minimum_required(VERSION 3.22.1)
project(trg_font_maker_android LANGUAGES CXX)

add_library(trg_font_maker SHARED
    native_bridge.cpp
    ../../../../src/TrueTypeWriter.cpp
)
target_include_directories(trg_font_maker PRIVATE ../../../../src)
target_compile_features(trg_font_maker PRIVATE cxx_std_17)
target_compile_options(trg_font_maker PRIVATE -O2 -Wall -Wextra)
target_link_libraries(trg_font_maker PRIVATE android log)
