#include "ImageBuffer.h"

#include <cassert>
#include <iostream>

using trg::photo::ImageBuffer;
using trg::photo::Pixel;

int main() {
    ImageBuffer source(2, 2);
    source.at(0, 0) = {255, 0, 0, 17};
    source.at(1, 0) = {0, 255, 0, 34};
    source.at(0, 1) = {0, 0, 255, 51};
    source.at(1, 1) = {12, 34, 56, 78};

    const ImageBuffer expanded = source.expanded2x();
    assert(expanded.width() == 4);
    assert(expanded.height() == 4);

    for (int y = 0; y < 2; ++y) {
        for (int x = 0; x < 2; ++x) {
            const Pixel expected = source.at(x, y);
            assert(expanded.at(x * 2, y * 2) == expected);
            assert(expanded.at(x * 2 + 1, y * 2) == expected);
            assert(expanded.at(x * 2, y * 2 + 1) == expected);
            assert(expanded.at(x * 2 + 1, y * 2 + 1) == expected);
        }
    }

    const ImageBuffer rotated = source.rotatedClockwise();
    assert(rotated.width() == 2 && rotated.height() == 2);
    assert(rotated.at(1, 0) == source.at(0, 0));
    assert(rotated.at(0, 0) == source.at(0, 1));

    const ImageBuffer flipped = source.flippedHorizontal();
    assert(flipped.at(0, 0) == source.at(1, 0));
    assert(flipped.at(1, 1) == source.at(0, 1));

    const ImageBuffer gray = source.grayscale();
    assert(gray.at(0, 0).r == gray.at(0, 0).g);
    assert(gray.at(0, 0).g == gray.at(0, 0).b);
    assert(gray.at(0, 0).a == source.at(0, 0).a);

    const ImageBuffer inverted = source.inverted();
    assert((inverted.at(0, 0) == Pixel{0, 255, 255, 17}));

    const ImageBuffer cropped = source.cropped(1, 0, 1, 2);
    assert(cropped.width() == 1 && cropped.height() == 2);
    assert(cropped.at(0, 1) == source.at(1, 1));

    std::cout << "TRG Photo Editor image-core tests passed.\n";
    return 0;
}
