# TRG Applications

Cross-platform applications built around the TRG ecosystem.

## Projects

- **TRG Photo Editor** — Qt/C++ image editor with a reusable, dependency-light image-processing core. Desktop and Android UI builds use the same C++ operations.
- **TRG Font Maker** — planned sibling application for glyph design, font project files, TTF export and device-compatible installation guidance.

The applications live in this repository initially so they can share the TRG toolchain and release automation. They are kept as independent CMake projects and can be split into separate repositories later without changing their source layout.

## Build

Each app has its own CMake project. See the app's README for prerequisites and target-specific instructions.
