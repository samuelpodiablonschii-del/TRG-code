#include "ImageBuffer.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>

namespace trg::photo {
namespace {

std::uint8_t clampChannel(double value) {
    return static_cast<std::uint8_t>(std::clamp(std::lround(value), 0L, 255L));
}

void validateDimensions(int width, int height) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Image dimensions must be positive.");
    }
    const auto count = static_cast<unsigned long long>(width) *
                       static_cast<unsigned long long>(height);
    if (count > 100000000ULL) {
        throw std::length_error("Image exceeds the 100 megapixel safety limit.");
    }
}

}

ImageBuffer::ImageBuffer(int width, int height, Pixel fill)
    : width_(width), height_(height) {
    validateDimensions(width, height);
    pixels_.assign(static_cast<std::size_t>(width) * static_cast<std::size_t>(height), fill);
}

Pixel& ImageBuffer::at(int x, int y) {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinate is outside the image.");
    }
    return pixels_[static_cast<std::size_t>(y) * width_ + x];
}

const Pixel& ImageBuffer::at(int x, int y) const {
    if (x < 0 || y < 0 || x >= width_ || y >= height_) {
        throw std::out_of_range("Pixel coordinate is outside the image.");
    }
    return pixels_[static_cast<std::size_t>(y) * width_ + x];
}

ImageBuffer ImageBuffer::expanded2x() const {
    if (empty()) return {};
    if (width_ > std::numeric_limits<int>::max() / 2 ||
        height_ > std::numeric_limits<int>::max() / 2) {
        throw std::length_error("Expanded image dimensions are too large.");
    }

    ImageBuffer output(width_ * 2, height_ * 2);
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const Pixel pixel = at(x, y);
            const int outX = x * 2;
            const int outY = y * 2;
            output.at(outX, outY) = pixel;
            output.at(outX + 1, outY) = pixel;
            output.at(outX, outY + 1) = pixel;
            output.at(outX + 1, outY + 1) = pixel;
        }
    }
    return output;
}

ImageBuffer ImageBuffer::scaledNearest(int newWidth, int newHeight) const {
    if (empty()) return {};
    validateDimensions(newWidth, newHeight);
    ImageBuffer output(newWidth, newHeight);
    for (int y = 0; y < newHeight; ++y) {
        const int sourceY = std::min(height_ - 1,
            static_cast<int>((static_cast<long long>(y) * height_) / newHeight));
        for (int x = 0; x < newWidth; ++x) {
            const int sourceX = std::min(width_ - 1,
                static_cast<int>((static_cast<long long>(x) * width_) / newWidth));
            output.at(x, y) = at(sourceX, sourceY);
        }
    }
    return output;
}

ImageBuffer ImageBuffer::cropped(int x, int y, int width, int height) const {
    if (empty()) return {};
    if (x < 0 || y < 0 || width <= 0 || height <= 0 ||
        x > width_ - width || y > height_ - height) {
        throw std::out_of_range("Crop rectangle is outside the image.");
    }

    ImageBuffer output(width, height);
    for (int row = 0; row < height; ++row) {
        for (int column = 0; column < width; ++column) {
            output.at(column, row) = at(x + column, y + row);
        }
    }
    return output;
}

ImageBuffer ImageBuffer::rotatedClockwise() const {
    if (empty()) return {};
    ImageBuffer output(height_, width_);
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            output.at(height_ - 1 - y, x) = at(x, y);
        }
    }
    return output;
}

ImageBuffer ImageBuffer::flippedHorizontal() const {
    if (empty()) return {};
    ImageBuffer output(width_, height_);
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            output.at(width_ - 1 - x, y) = at(x, y);
        }
    }
    return output;
}

ImageBuffer ImageBuffer::flippedVertical() const {
    if (empty()) return {};
    ImageBuffer output(width_, height_);
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            output.at(x, height_ - 1 - y) = at(x, y);
        }
    }
    return output;
}

ImageBuffer ImageBuffer::grayscale() const {
    if (empty()) return {};
    ImageBuffer output(width_, height_);
    for (std::size_t i = 0; i < pixels_.size(); ++i) {
        const Pixel p = pixels_[i];
        const auto gray = clampChannel(0.2126 * p.r + 0.7152 * p.g + 0.0722 * p.b);
        output.pixels_[i] = {gray, gray, gray, p.a};
    }
    return output;
}

ImageBuffer ImageBuffer::inverted() const {
    if (empty()) return {};
    ImageBuffer output(width_, height_);
    for (std::size_t i = 0; i < pixels_.size(); ++i) {
        const Pixel p = pixels_[i];
        output.pixels_[i] = {
            static_cast<std::uint8_t>(255 - p.r),
            static_cast<std::uint8_t>(255 - p.g),
            static_cast<std::uint8_t>(255 - p.b),
            p.a
        };
    }
    return output;
}

ImageBuffer ImageBuffer::adjusted(int brightness, int contrast) const {
    if (empty()) return {};
    const double factor = (contrast + 100.0) / 100.0;
    const double contrastFactor = factor * factor;
    ImageBuffer output(width_, height_);
    for (std::size_t i = 0; i < pixels_.size(); ++i) {
        const Pixel p = pixels_[i];
        const auto adjust = [&](std::uint8_t channel) {
            const double normalized = channel / 255.0;
            const double contrasted = ((normalized - 0.5) * contrastFactor + 0.5) * 255.0;
            return clampChannel(contrasted + brightness);
        };
        output.pixels_[i] = {adjust(p.r), adjust(p.g), adjust(p.b), p.a};
    }
    return output;
}

}
