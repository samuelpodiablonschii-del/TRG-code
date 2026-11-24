# TRG Photo Editor

A cross-platform image editor built with C++17 and Qt 6.

## Current foundation

- Open and save common raster formats supported by Qt (PNG, JPEG, BMP, WebP when the Qt image plugin is available).
- Lossless 2× pixel expansion: every source pixel is copied into a 2×2 block with exactly the same RGBA value.
- Rotate, flip, crop, grayscale, invert, brightness and contrast operations.
- Zoomable canvas, layer list foundation, undo and redo.
- Shared C++ image core with tests independent of Qt.

## Build

Requirements:
- CMake 3.21+
- C++17 compiler
- Qt 6.5+ with Core, Gui and Widgets

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
ctest --test-dir build --output-on-failure
```

For Android, configure CMake with a Qt 6 Android kit and the Android SDK/NDK selected by Qt Creator. The same image core is used by the Android target. The UI uses touch-sized controls and adapts its splitter layout to narrow screens.

## Pixel expansion

For a source image of width W and height H, the 2× operation creates an image of width 2W and height 2H. Source pixel (x,y) is copied unchanged to (2x,2y), (2x+1,2y), (2x,2y+1), and (2x+1,2y+1). No interpolation is performed.
