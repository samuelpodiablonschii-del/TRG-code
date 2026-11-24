cmake_minimum_required(VERSION 3.21)
project(TRGPhotoEditor VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_library(trg_photo_core
    src/ImageBuffer.cpp
)
target_include_directories(trg_photo_core PUBLIC src)

add_executable(trg_photo_core_tests
    tests/ImageBufferTests.cpp
)
target_link_libraries(trg_photo_core_tests PRIVATE trg_photo_core)

if(BUILD_TESTING)
    add_test(NAME trg_photo_core_tests COMMAND trg_photo_core_tests)
endif()

option(TRG_PHOTO_EDITOR_BUILD_UI "Build the Qt desktop/mobile application" ON)
if(TRG_PHOTO_EDITOR_BUILD_UI)
    find_package(Qt6 6.5 QUIET COMPONENTS Core Gui Widgets)
    if(Qt6_FOUND)
        add_executable(TRGPhotoEditor
            src/main.cpp
            src/PhotoEditorWindow.cpp
            src/PhotoEditorWindow.h
        )
        target_link_libraries(TRGPhotoEditor PRIVATE
            trg_photo_core
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
        )
        target_compile_definitions(TRGPhotoEditor PRIVATE QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII)
    else()
        message(STATUS "Qt 6 was not found; building the tested C++ image core only.")
    endif()
endif()
