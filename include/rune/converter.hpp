#pragma once

#include <string>
#include "cell.hpp"
#include "ramp.hpp"
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

        // HSL color representation (Hue, Saturation, Lightness)
        struct HSL {
            float h; // Hue in degrees (0-360)
            float s; // Saturation as ratio (0.0-1.0)
            float l; // Lightness as ratio (0.0-1.0)
        };

        // Container for raw image pixel data
        struct ImageBuffer {
            std::vector<uint8_t> pixels; // Raw pixel data in RGB format
            int width;                   // Image width in pixels
            int height;                  // Image height in pixels
            int channels;                // Number of color channels (typically 3 for RGB)
        };

        // Represents a single frame converted to ASCII format
        struct AsciiFrame {
            ImageBuffer image_buffer;         // Original image data
            std::vector<rune::Cell> cells;    // ASCII cells with glyphs and colors
            std::string html = "";            // HTML representation of the frame
        };

        // Converts a single image frame to ASCII art
        AsciiFrame convert_frame_to_ascii(const std::string& filename, int target_width, const rune::Ramp& ramp, float threshold = 0.0f);

        // Generates HTML representation of an ASCII frame with color spans
        void add_html(AsciiFrame& ascii_frame);

        // Converts a video file to ASCII frames and saves to output folder
        void convert_video_to_ascii(const std::string& filename, int target_width, int target_fps, const std::string& output_folder, const rune::Ramp& ramp, float threshold = 0.0f);

        // Converts a single image to ASCII and saves to output folder
        void convert_image_to_ascii(const std::string& filename, int target_width, const std::string& output_folder, const rune::Ramp& ramp, float threshold = 0.0f);

        // Loads image from file and returns pixel data
        ImageBuffer load_image_pixels(const std::string& filename);

        // Resizes image to target width while maintaining aspect ratio
        ImageBuffer resize_image_pixels(const ImageBuffer& image_buffer, int target_width);

        // Converts image pixels to ASCII cells with glyphs and colors
        std::vector<rune::Cell> pixels_to_cells (const ImageBuffer& image_buffer, const rune::Ramp& ramp, float threshold = 0.0f);

        // Converts RGB color values to HSL color space
        HSL rgb_to_hsl(int r, int g, int b);

        // Displays a progress bar in the terminal
        void print_progress(int current, int total);

    } // namespace converter
} // namespace rune
