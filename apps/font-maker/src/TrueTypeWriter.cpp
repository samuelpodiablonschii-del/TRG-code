#include "TrueTypeWriter.h"

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <limits>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

namespace trg::font {
namespace {

using Bytes = std::vector<std::uint8_t>;

void putU16(Bytes& out, std::uint16_t value) {
    out.push_back(static_cast<std::uint8_t>(value >> 8));
    out.push_back(static_cast<std::uint8_t>(value));
}
void putS16(Bytes& out, std::int16_t value) {
    putU16(out, static_cast<std::uint16_t>(value));
}
void putU32(Bytes& out, std::uint32_t value) {
    out.push_back(static_cast<std::uint8_t>(value >> 24));
    out.push_back(static_cast<std::uint8_t>(value >> 16));
    out.push_back(static_cast<std::uint8_t>(value >> 8));
    out.push_back(static_cast<std::uint8_t>(value));
}
void putU64(Bytes& out, std::uint64_t value) {
    putU32(out, static_cast<std::uint32_t>(value >> 32));
    putU32(out, static_cast<std::uint32_t>(value));
}
void putTag(Bytes& out, const std::string& tag) {
    for (int i = 0; i < 4; ++i) out.push_back(static_cast<std::uint8_t>(tag[static_cast<std::size_t>(i)]));
}
void patchU32(Bytes& out, std::size_t offset, std::uint32_t value) {
    out.at(offset) = static_cast<std::uint8_t>(value >> 24);
    out.at(offset + 1) = static_cast<std::uint8_t>(value >> 16);
    out.at(offset + 2) = static_cast<std::uint8_t>(value >> 8);
    out.at(offset + 3) = static_cast<std::uint8_t>(value);
}
void pad4(Bytes& out) {
    while (out.size() % 4 != 0) out.push_back(0);
}
std::uint32_t checksum(const Bytes& bytes) {
    std::uint32_t sum = 0;
    for (std::size_t i = 0; i < bytes.size(); i += 4) {
        std::uint32_t word = 0;
        for (std::size_t j = 0; j < 4; ++j) {
            word <<= 8;
            if (i + j < bytes.size()) word |= bytes[i + j];
        }
        sum += word;
    }
    return sum;
}
std::vector<std::uint16_t> utf16(const std::string& text) {
    std::vector<std::uint16_t> result;
    for (std::size_t i = 0; i < text.size();) {
        const auto first = static_cast<std::uint8_t>(text[i]);
        std::uint32_t cp = 0xfffd;
        std::size_t count = 1;
        if (first < 0x80) cp = first;
        else if ((first & 0xe0) == 0xc0 && i + 1 < text.size()) {
            cp = ((first & 0x1f) << 6) | (static_cast<std::uint8_t>(text[i + 1]) & 0x3f);
            count = 2;
        } else if ((first & 0xf0) == 0xe0 && i + 2 < text.size()) {
            cp = ((first & 0x0f) << 12) |
                 ((static_cast<std::uint8_t>(text[i + 1]) & 0x3f) << 6) |
                 (static_cast<std::uint8_t>(text[i + 2]) & 0x3f);
            count = 3;
        } else if ((first & 0xf8) == 0xf0 && i + 3 < text.size()) {
            cp = ((first & 0x07) << 18) |
                 ((static_cast<std::uint8_t>(text[i + 1]) & 0x3f) << 12) |
                 ((static_cast<std::uint8_t>(text[i + 2]) & 0x3f) << 6) |
                 (static_cast<std::uint8_t>(text[i + 3]) & 0x3f);
            count = 4;
        }
        i += count;
        if (cp <= 0xffff) result.push_back(static_cast<std::uint16_t>(cp));
        else if (cp <= 0x10ffff) {
            cp -= 0x10000;
            result.push_back(static_cast<std::uint16_t>(0xd800 + (cp >> 10)));
            result.push_back(static_cast<std::uint16_t>(0xdc00 + (cp & 0x3ff)));
        }
    }
    return result;
}
Bytes makeNameTable(const std::string& family, const std::string& style) {
    const std::string full = family + " " + style;
    std::string postScript;
    for (unsigned char c : full) {
        if (std::isalnum(c)) postScript.push_back(static_cast<char>(c));
        else if (c == ' ' || c == '-') postScript.push_back('-');
    }
    if (postScript.empty()) postScript = "TRGCustom-Regular";

    const std::array<std::pair<std::uint16_t, std::string>, 4> names{{
        {1, family}, {2, style}, {4, full}, {6, postScript}
    }};
    Bytes storage;
    Bytes records;
    for (const auto& entry : names) {
        const auto units = utf16(entry.second);
        const std::uint16_t length = static_cast<std::uint16_t>(units.size() * 2);
        putU16(records, 3);
        putU16(records, 1);
        putU16(records, 0x0409);
        putU16(records, entry.first);
        putU16(records, length);
        putU16(records, static_cast<std::uint16_t>(storage.size()));
        for (auto unit : units) putU16(storage, unit);
    }
    Bytes table;
    putU16(table, 0);
    putU16(table, static_cast<std::uint16_t>(names.size()));
    putU16(table, static_cast<std::uint16_t>(6 + records.size()));
    table.insert(table.end(), records.begin(), records.end());
    table.insert(table.end(), storage.begin(), storage.end());
    return table;
}

struct BuiltGlyph {
    Bytes data;
    std::uint16_t points{0};
    std::uint16_t contours{0};
    std::int16_t xMin{0}, yMin{0}, xMax{0}, yMax{0};
};

BuiltGlyph makeGlyph(const FontGlyph& glyph) {
    BuiltGlyph built;
    std::vector<GlyphPoint> points;
    std::vector<std::uint16_t> ends;
    for (const auto& contour : glyph.contours) {
        if (contour.points.size() < 3) continue;
        for (const auto& point : contour.points) points.push_back(point);
        ends.push_back(static_cast<std::uint16_t>(points.size() - 1));
    }
    if (points.size() > 65535 || ends.size() > 32767) {
        throw std::runtime_error("A glyph contains too many points or contours.");
    }
    built.points = static_cast<std::uint16_t>(points.size());
    built.contours = static_cast<std::uint16_t>(ends.size());

    if (!points.empty()) {
        auto minX = points.front().x, maxX = points.front().x;
        auto minY = points.front().y, maxY = points.front().y;
        for (const auto& point : points) {
            minX = std::min(minX, point.x); maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y); maxY = std::max(maxY, point.y);
        }
        built.xMin = minX; built.xMax = maxX; built.yMin = minY; built.yMax = maxY;
    }

    putS16(built.data, static_cast<std::int16_t>(built.contours));
    putS16(built.data, built.xMin); putS16(built.data, built.yMin);
    putS16(built.data, built.xMax); putS16(built.data, built.yMax);
    for (auto end : ends) putU16(built.data, end);
    putU16(built.data, 0); // No TrueType hinting instructions.
    for (const auto& point : points) built.data.push_back(point.onCurve ? 0x01 : 0x00);

    std::int32_t previous = 0;
    for (const auto& point : points) {
        const std::int32_t delta = static_cast<std::int32_t>(point.x) - previous;
        putS16(built.data, static_cast<std::int16_t>(delta));
        previous = point.x;
    }
    previous = 0;
    for (const auto& point : points) {
        const std::int32_t delta = static_cast<std::int32_t>(point.y) - previous;
        putS16(built.data, static_cast<std::int16_t>(delta));
        previous = point.y;
    }
    pad4(built.data);
    return built;
}

Bytes makeCmap(const std::vector<FontGlyph>& glyphs) {
    struct Mapping { std::uint16_t codepoint; std::uint16_t glyphIndex; };
    std::vector<Mapping> mappings;
    for (std::size_t i = 0; i < glyphs.size(); ++i) {
        const auto cp = glyphs[i].codepoint;
        if (cp <= 0xffff && !(cp >= 0xd800 && cp <= 0xdfff)) {
            mappings.push_back({static_cast<std::uint16_t>(cp), static_cast<std::uint16_t>(i)});
        }
    }
    std::sort(mappings.begin(), mappings.end(), [](const Mapping& a, const Mapping& b) {
        return a.codepoint < b.codepoint;
    });
    const std::size_t segmentCount = mappings.size() + 1;
    const std::size_t length = 16 + segmentCount * 8;
    if (length > 65535) throw std::runtime_error("Too many BMP glyph mappings for a format 4 cmap.");

    Bytes sub;
    putU16(sub, 4);
    putU16(sub, static_cast<std::uint16_t>(length));
    putU16(sub, 0);
    putU16(sub, static_cast<std::uint16_t>(segmentCount * 2));
    std::size_t power = 1, selector = 0;
    while (power * 2 <= segmentCount) { power *= 2; ++selector; }
    putU16(sub, static_cast<std::uint16_t>(power * 2));
    putU16(sub, static_cast<std::uint16_t>(selector));
    putU16(sub, static_cast<std::uint16_t>(segmentCount * 2 - power * 2));
    for (const auto& map : mappings) putU16(sub, map.codepoint);
    putU16(sub, 0xffff);
    putU16(sub, 0);
    for (const auto& map : mappings) putU16(sub, map.codepoint);
    putU16(sub, 0xffff);
    for (const auto& map : mappings) putU16(sub, static_cast<std::uint16_t>(map.glyphIndex - map.codepoint));
    putU16(sub, 1);
    for (std::size_t i = 0; i < segmentCount; ++i) putU16(sub, 0);

    Bytes table;
    putU16(table, 0);
    putU16(table, 1);
    putU16(table, 3);
    putU16(table, 1);
    putU32(table, 12);
    table.insert(table.end(), sub.begin(), sub.end());
    return table;
}

Bytes makeHead(std::uint16_t unitsPerEm, std::int16_t xMin, std::int16_t yMin,
               std::int16_t xMax, std::int16_t yMax) {
    Bytes t;
    putU32(t, 0x00010000); putU32(t, 0x00010000); putU32(t, 0);
    putU32(t, 0x5f0f3cf5); putU16(t, 0x000b); putU16(t, unitsPerEm);
    putU64(t, 0); putU64(t, 0);
    putS16(t, xMin); putS16(t, yMin); putS16(t, xMax); putS16(t, yMax);
    putU16(t, 0); putU16(t, 8); putS16(t, 2); putS16(t, 1); putS16(t, 0);
    return t;
}

Bytes makeHhea(const FontProject& project, const std::vector<FontGlyph>& glyphs,
               std::int16_t xMaxExtent) {
    std::uint16_t maxAdvance = 0;
    std::int16_t minLsb = std::numeric_limits<std::int16_t>::max();
    std::int16_t minRsb = std::numeric_limits<std::int16_t>::max();
    for (const auto& glyph : glyphs) {
        maxAdvance = std::max(maxAdvance, glyph.advanceWidth);
        minLsb = std::min(minLsb, glyph.leftSideBearing);
        minRsb = std::min<std::int16_t>(minRsb,
            static_cast<std::int16_t>(glyph.advanceWidth - glyph.leftSideBearing));
    }
    if (glyphs.empty()) { minLsb = 0; minRsb = 0; }
    Bytes t;
    putU32(t, 0x00010000);
    putS16(t, project.ascender); putS16(t, project.descender); putS16(t, 0);
    putU16(t, maxAdvance); putS16(t, minLsb); putS16(t, minRsb); putS16(t, xMaxExtent);
    putS16(t, 1); putS16(t, 0); putS16(t, 0);
    for (int i = 0; i < 4; ++i) putS16(t, 0);
    putS16(t, 0);
    putU16(t, static_cast<std::uint16_t>(glyphs.size()));
    return t;
}

Bytes makeMaxp(std::uint16_t glyphCount, std::uint16_t maxPoints, std::uint16_t maxContours) {
    Bytes t;
    putU32(t, 0x00010000);
    putU16(t, glyphCount); putU16(t, maxPoints); putU16(t, maxContours);
    putU16(t, 0); putU16(t, 0); putU16(t, 2);
    putU16(t, 0); putU16(t, 0); putU16(t, 0); putU16(t, 0);
    putU16(t, 0); putU16(t, 0); putU16(t, 0);
    return t;
}

Bytes makeOs2(const FontProject& project, const std::vector<FontGlyph>& glyphs) {
    std::uint16_t first = 0xffff, last = 0;
    std::uint32_t widthTotal = 0;
    for (const auto& glyph : glyphs) {
        if (glyph.codepoint <= 0xffff) {
            first = std::min(first, static_cast<std::uint16_t>(glyph.codepoint));
            last = std::max(last, static_cast<std::uint16_t>(glyph.codepoint));
        }
        widthTotal += glyph.advanceWidth;
    }
    if (first == 0xffff) first = 0;
    const std::uint16_t average = glyphs.empty() ? 0 :
        static_cast<std::uint16_t>(widthTotal / glyphs.size());

    Bytes t;
    putU16(t, 1); putS16(t, static_cast<std::int16_t>(average));
    putU16(t, 400); putU16(t, 5); putU16(t, 0);
    for (int i = 0; i < 8; ++i) putS16(t, 0);
    putS16(t, 50); putS16(t, 250); putS16(t, 0);
    for (int i = 0; i < 10; ++i) t.push_back(0);
    for (int i = 0; i < 4; ++i) putU32(t, 0);
    t.insert(t.end(), {'T','R','G',' '});
    putU16(t, 0x0040); putU16(t, first); putU16(t, last);
    putS16(t, project.ascender); putS16(t, project.descender); putS16(t, 0);
    putU16(t, static_cast<std::uint16_t>(std::max<int>(0, project.ascender)));
    putU16(t, static_cast<std::uint16_t>(std::max<int>(0, -project.descender)));
    putU32(t, 0); putU32(t, 0);
    return t;
}

Bytes makePost() {
    Bytes t;
    putU32(t, 0x00030000); putU32(t, 0);
    putS16(t, -100); putS16(t, 50); putU32(t, 0);
    for (int i = 0; i < 4; ++i) putU32(t, 0);
    return t;
}

}

bool exportTrueType(const FontProject& project, const std::string& outputPath,
                    std::string* errorMessage) {
    try {
        if (project.unitsPerEm < 16 || project.unitsPerEm > 16384) {
            throw std::runtime_error("Units per em must be between 16 and 16384.");
        }
        std::vector<FontGlyph> glyphs = project.glyphs;
        if (glyphs.empty() || glyphs.front().codepoint != 0) {
            FontGlyph notdef;
            notdef.codepoint = 0;
            notdef.advanceWidth = 600;
            notdef.contours = {{{{80, 0, true}, {520, 0, true}, {520, 700, true}, {80, 700, true}}}};
            glyphs.insert(glyphs.begin(), notdef);
        }
        std::sort(glyphs.begin(), glyphs.end(), [](const FontGlyph& a, const FontGlyph& b) {
            return a.codepoint < b.codepoint;
        });
        if (glyphs.size() > 32000) throw std::runtime_error("The font contains too many glyphs.");
        std::set<std::uint32_t> codepoints;
        for (const auto& glyph : glyphs) {
            if (glyph.codepoint > 0xffff || (glyph.codepoint >= 0xd800 && glyph.codepoint <= 0xdfff)) {
                throw std::runtime_error("This first TTF exporter supports Unicode BMP codepoints only.");
            }
            if (!codepoints.insert(glyph.codepoint).second) throw std::runtime_error("Duplicate glyph codepoint.");
            for (const auto& contour : glyph.contours) {
                for (const auto& point : contour.points) {
                    if (point.x < -32768 || point.x > 32767 || point.y < -32768 || point.y > 32767) {
                        throw std::runtime_error("Glyph coordinates are outside the TrueType range.");
                    }
                }
            }
        }

        std::map<std::string, Bytes> tables;
        std::vector<BuiltGlyph> builtGlyphs;
        Bytes glyf, loca;
        std::uint16_t maxPoints = 0, maxContours = 0;
        std::int16_t globalXMin = 0, globalYMin = 0, globalXMax = 0, globalYMax = 0;
        std::int16_t xMaxExtent = 0;
        bool hasBounds = false;
        for (const auto& glyph : glyphs) {
            BuiltGlyph built = makeGlyph(glyph);
            maxPoints = std::max(maxPoints, built.points);
            maxContours = std::max(maxContours, built.contours);
            if (built.points > 0) {
                if (!hasBounds) {
                    globalXMin = built.xMin; globalYMin = built.yMin;
                    globalXMax = built.xMax; globalYMax = built.yMax; hasBounds = true;
                } else {
                    globalXMin = std::min(globalXMin, built.xMin);
                    globalYMin = std::min(globalYMin, built.yMin);
                    globalXMax = std::max(globalXMax, built.xMax);
                    globalYMax = std::max(globalYMax, built.yMax);
                }
                xMaxExtent = std::max<std::int16_t>(xMaxExtent,
                    static_cast<std::int16_t>(glyph.leftSideBearing + built.xMax));
            }
            putU32(loca, static_cast<std::uint32_t>(glyf.size()));
            glyf.insert(glyf.end(), built.data.begin(), built.data.end());
            builtGlyphs.push_back(std::move(built));
        }
        putU32(loca, static_cast<std::uint32_t>(glyf.size()));

        Bytes hmtx;
        for (const auto& glyph : glyphs) {
            putU16(hmtx, glyph.advanceWidth);
            putS16(hmtx, glyph.leftSideBearing);
        }

        tables["OS/2"] = makeOs2(project, glyphs);
        tables["cmap"] = makeCmap(glyphs);
        tables["glyf"] = std::move(glyf);
        tables["head"] = makeHead(project.unitsPerEm, globalXMin, globalYMin, globalXMax, globalYMax);
        tables["hhea"] = makeHhea(project, glyphs, xMaxExtent);
        tables["hmtx"] = std::move(hmtx);
        tables["loca"] = std::move(loca);
        tables["maxp"] = makeMaxp(static_cast<std::uint16_t>(glyphs.size()), maxPoints, maxContours);
        tables["name"] = makeNameTable(project.familyName, project.styleName);
        tables["post"] = makePost();

        const std::uint16_t tableCount = static_cast<std::uint16_t>(tables.size());
        Bytes font;
        putU32(font, 0x00010000);
        putU16(font, tableCount);
        std::size_t power = 1, selector = 0;
        while (power * 2 <= tableCount) { power *= 2; ++selector; }
        putU16(font, static_cast<std::uint16_t>(power * 16));
        putU16(font, static_cast<std::uint16_t>(selector));
        putU16(font, static_cast<std::uint16_t>(tableCount * 16 - power * 16));

        struct DirectoryEntry { std::string tag; std::uint32_t sum; std::uint32_t offset; std::uint32_t length; };
        std::vector<DirectoryEntry> directory;
        std::uint32_t offset = static_cast<std::uint32_t>(12 + tableCount * 16);
        for (auto& table : tables) {
            pad4(table.second);
            directory.push_back({table.first, checksum(table.second), offset,
                                  static_cast<std::uint32_t>(table.second.size())});
            offset += static_cast<std::uint32_t>(table.second.size());
        }
        for (const auto& entry : directory) {
            putTag(font, entry.tag);
            putU32(font, entry.sum);
            putU32(font, entry.offset);
            putU32(font, entry.length);
        }
        std::uint32_t headOffset = 0;
        for (const auto& entry : directory) {
            if (entry.tag == "head") headOffset = entry.offset;
            const auto& data = tables.at(entry.tag);
            font.insert(font.end(), data.begin(), data.end());
        }
        const std::uint32_t adjustment = 0xb1b0afbaU - checksum(font);
        patchU32(font, static_cast<std::size_t>(headOffset) + 8, adjustment);

        std::ofstream output(outputPath, std::ios::binary);
        if (!output) throw std::runtime_error("Could not open the output file.");
        output.write(reinterpret_cast<const char*>(font.data()), static_cast<std::streamsize>(font.size()));
        if (!output.good()) throw std::runtime_error("Could not write the TrueType font file.");
        return true;
    } catch (const std::exception& error) {
        if (errorMessage) *errorMessage = error.what();
        return false;
    }
}

}
