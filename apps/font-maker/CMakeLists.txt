cmake_minimum_required(VERSION 3.21)
project(TRGFontMaker VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_library(trg_font_core
    src/TrueTypeWriter.cpp
)
target_include_directories(trg_font_core PUBLIC src)

add_executable(trg_font_core_tests
    tests/TrueTypeWriterTests.cpp
)
target_link_libraries(trg_font_core_tests PRIVATE trg_font_core)

if(BUILD_TESTING)
    add_test(NAME trg_font_core_tests COMMAND trg_font_core_tests)
endif()

option(TRG_FONT_MAKER_BUILD_UI "Build the Qt desktop/mobile application" ON)
if(TRG_FONT_MAKER_BUILD_UI)
    find_package(Qt6 6.5 QUIET COMPONENTS Core Gui Widgets)
    if(Qt6_FOUND)
        qt_add_executable(TRGFontMaker
            src/main.cpp
            src/FontMakerWindow.cpp
            src/FontMakerWindow.h
            src/FontCanvas.cpp
            src/FontCanvas.h
        )
        set_target_properties(TRGFontMaker PROPERTIES QT_ANDROID_PACKAGE_NAME "com.techrivergames.trgfontmaker")
        target_link_libraries(TRGFontMaker PRIVATE
            trg_font_core
            Qt6::Core
            Qt6::Gui
            Qt6::Widgets
        )
    else()
        message(STATUS "Qt 6 was not found; building the tested C++ font core only.")
    endif()
endif()
