cmake_minimum_required(VERSION 3.22.1)
project(trg_photo_editor_android LANGUAGES CXX)

add_library(trg_photo_editor SHARED
    native_bridge.cpp
    ../../../../src/ImageBuffer.cpp
)
target_include_directories(trg_photo_editor PRIVATE ../../../../src)
target_compile_features(trg_photo_editor PRIVATE cxx_std_17)
target_compile_options(trg_photo_editor PRIVATE -O2 -Wall -Wextra)
target_link_libraries(trg_photo_editor PRIVATE android log jnigraphics)
