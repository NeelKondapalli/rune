#pragma once

#include <string>
#include "rune/cell.hpp"
#include <cstdint>
#include <vector>

#include <string>
#include <algorithm>
#include <stdexcept>
#include <cstdint>

namespace rune {

    namespace converter {
    
        struct HSL {
            float h;
            float s;
            float l;
        };

        struct ImageBuffer {
            std::vector<uint8_t> pixels;
            int width;
            int height;
            int channels;
        };

        void convert_image_to_ascii(const std::string& filename, int target_width, std::ostream& out);

        ImageBuffer load_image_pixels(const std::string& filename);

        ImageBuffer resize_image_pixels(const ImageBuffer& image_buffer, int target_width);

        std::vector<rune::Cell> pixels_to_cells (const ImageBuffer& image_buffer);

        HSL rgb_to_hsl(int r, int g, int b);

    } // namespace converter
} // namespace rune
