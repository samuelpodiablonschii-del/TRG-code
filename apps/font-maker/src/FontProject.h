#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace trg::font {

struct GlyphPoint {
    std::int16_t x{0};
    std::int16_t y{0};
    bool onCurve{true};
};

struct GlyphContour {
    std::vector<GlyphPoint> points;
};

struct FontGlyph {
    std::uint32_t codepoint{0};
    std::uint16_t advanceWidth{600};
    std::int16_t leftSideBearing{0};
    std::vector<GlyphContour> contours;
};

struct FontProject {
    std::string familyName{"TRG Custom"};
    std::string styleName{"Regular"};
    std::uint16_t unitsPerEm{1000};
    std::int16_t ascender{800};
    std::int16_t descender{-200};
    std::vector<FontGlyph> glyphs;
};

}
