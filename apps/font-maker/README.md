# TRG Font Maker

A C++ font-design project for desktop and Android.

## Current foundation

- A portable C++ font project model with Unicode BMP glyph mapping.
- Polygon-outline glyph data in a 1000-unit em square.
- A native TrueType (TTF) writer for simple glyph contours.
- A Qt 6 editor UI for drawing glyph outlines, setting advances, saving project files and exporting TTF.
- Android can preview and export fonts. System-wide font changes are controlled by Android and device manufacturers; there is no universal public API that lets an ordinary app replace the system font on every device. The app will not root the device or modify protected system files.

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

Glyph contours are currently polygonal. Curve editing, kerning, advanced OpenType tables and broader script shaping are later milestones.
