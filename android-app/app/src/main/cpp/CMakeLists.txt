cmake_minimum_required(VERSION 3.22.1)
project(trg_android LANGUAGES CXX)

add_library(trg SHARED native_bridge.cpp)
target_compile_features(trg PRIVATE cxx_std_17)
target_compile_options(trg PRIVATE -O2 -Wall -Wextra -Wno-unused-parameter)
target_link_libraries(trg PRIVATE log)
