#pragma once

#include <string>
#include "rune/cell.hpp"
#include <cstdint>
#include <vector>

#include <string>
#include <algorithm>
#include <stdexcept>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <iomanip>
#include <zlib.h>


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

        struct AsciiFrame {
            ImageBuffer image_buffer;
            std::vector<rune::Cell> cells;
            std::string html = "";
        };

        AsciiFrame convert_image_to_ascii(const std::string& filename, int target_width);

        void add_html(AsciiFrame& ascii_frame);

        void convert_video_to_ascii(const std::string& filename, int target_width, int target_fps, const std::string& output_folder);

        ImageBuffer load_image_pixels(const std::string& filename);

        ImageBuffer resize_image_pixels(const ImageBuffer& image_buffer, int target_width);

        std::vector<rune::Cell> pixels_to_cells (const ImageBuffer& image_buffer);

        HSL rgb_to_hsl(int r, int g, int b);

        void print_progress(int current, int total);

    } // namespace converter
} // namespace rune
