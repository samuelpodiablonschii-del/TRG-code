#include "TrueTypeWriter.h"

#include <cassert>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

using namespace trg::font;

int main() {
    FontProject project;
    project.familyName = "TRG Test";
    project.styleName = "Regular";

    FontGlyph notdef;
    notdef.codepoint = 0;
    notdef.advanceWidth = 600;
    notdef.contours = {{{{80, 0, true}, {520, 0, true}, {520, 700, true}, {80, 700, true}}}};
    project.glyphs.push_back(notdef);

    FontGlyph space;
    space.codepoint = 32;
    space.advanceWidth = 300;
    project.glyphs.push_back(space);

    FontGlyph letterA;
    letterA.codepoint = 65;
    letterA.advanceWidth = 700;
    letterA.leftSideBearing = 20;
    letterA.contours = {{{{50, 0, true}, {350, 750, true}, {650, 0, true}}}};
    project.glyphs.push_back(letterA);

    const std::string path = "trg-font-core-test.ttf";
    std::string error;
    assert(exportTrueType(project, path, &error));

    std::ifstream input(path, std::ios::binary);
    assert(input.good());
    const std::vector<unsigned char> bytes(
        (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    assert(bytes.size() > 200);
    assert(bytes[0] == 0x00 && bytes[1] == 0x01 && bytes[2] == 0x00 && bytes[3] == 0x00);

    const std::string raw(bytes.begin(), bytes.end());
    assert(raw.find("cmap") != std::string::npos);
    assert(raw.find("glyf") != std::string::npos);
    assert(raw.find("head") != std::string::npos);
    assert(raw.find("name") != std::string::npos);

    std::remove(path.c_str());
    std::cout << "TRG Font Maker TrueType export tests passed.\n";
    return 0;
}
