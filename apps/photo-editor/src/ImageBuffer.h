#pragma once

#include <cstdint>
#include <vector>

namespace trg::photo {

struct Pixel {
    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};

    bool operator==(const Pixel& other) const noexcept {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

class ImageBuffer {
public:
    ImageBuffer() = default;
    ImageBuffer(int width, int height, Pixel fill = {});

    int width() const noexcept { return width_; }
    int height() const noexcept { return height_; }
    bool empty() const noexcept { return width_ <= 0 || height_ <= 0 || pixels_.empty(); }

    Pixel& at(int x, int y);
    const Pixel& at(int x, int y) const;
    const std::vector<Pixel>& pixels() const noexcept { return pixels_; }

    ImageBuffer expanded2x() const;
    ImageBuffer scaledNearest(int newWidth, int newHeight) const;
    ImageBuffer cropped(int x, int y, int width, int height) const;
    ImageBuffer rotatedClockwise() const;
    ImageBuffer flippedHorizontal() const;
    ImageBuffer flippedVertical() const;
    ImageBuffer grayscale() const;
    ImageBuffer inverted() const;
    ImageBuffer adjusted(int brightness, int contrast) const;

private:
    int width_{0};
    int height_{0};
    std::vector<Pixel> pixels_;
};

}
