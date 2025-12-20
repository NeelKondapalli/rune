#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_resize.h"
#include "rune/ramp.hpp"
#include "rune/converter.hpp"
#include "rune/json.hpp"
#include <iostream>


namespace rune {
    namespace converter {
        void convert_image_to_ascii(const std::string& filename, int target_width, std::ostream& out) {
            ImageBuffer image_buffer = load_image_pixels(filename);
            ImageBuffer resized_image_buffer = resize_image_pixels(image_buffer, target_width);
            std::vector<rune::Cell> cells = pixels_to_cells(resized_image_buffer);
            json::write_json(out, resized_image_buffer, cells);
        }

        ImageBuffer load_image_pixels(const std::string& filename) {
            ImageBuffer image_buffer;
            int width, height, channels;

            unsigned char* raw_pixels = stbi_load(
                filename.c_str(),
                &width,
                &height,
                &channels,
                3
            );

            if (!raw_pixels) {
                throw std::runtime_error("Failed to load image");
            }

            size_t size = width * height * 3;

            ImageBuffer buffer;
            buffer.width = width;
            buffer.height = height;
            buffer.channels = 3;
            buffer.pixels.assign(raw_pixels, raw_pixels + size);

            stbi_image_free(raw_pixels);
        
            return buffer;
        }

        ImageBuffer resize_image_pixels(const ImageBuffer& image_buffer, int target_width) {
            ImageBuffer resized_image_buffer;
            int new_height = image_buffer.height * target_width / image_buffer.width;

            resized_image_buffer.pixels.resize(target_width * new_height * image_buffer.channels);
            
            stbir_resize_uint8(
                image_buffer.pixels.data(),
                image_buffer.width,
                image_buffer.height,
                0,
                resized_image_buffer.pixels.data(),
                target_width,
                new_height,
                0, image_buffer.channels);

            resized_image_buffer.width = target_width;
            resized_image_buffer.height = new_height;
            resized_image_buffer.channels = image_buffer.channels;

            return resized_image_buffer;
        }

        std::vector<rune::Cell> pixels_to_cells(const ImageBuffer& image_buffer) {

            std::vector<rune::Cell> cells;

            for (int y = 0; y < image_buffer.height; y+=2) {
                for (int x = 0; x < image_buffer.width; x++) {
                    rune::Cell cell;
                    int idx = (y * image_buffer.width + x) * image_buffer.channels;
                    int r = image_buffer.pixels[idx];
                    int g = image_buffer.pixels[idx + 1];
                    int b = image_buffer.pixels[idx + 2];

                    HSL hsl = rgb_to_hsl(r, g, b);

                    float t = std::clamp(hsl.l, 0.0f, 1.0f);

                    int ascii_index = static_cast<int>(t * (rune::ramps::SIMPLE.chars.size() - 1));

                    cell.glyph = rune::ramps::SIMPLE.chars[ascii_index];
                    cell.h = hsl.h;
                    cell.s = hsl.s;
                    cell.l = hsl.l;
                    cells.push_back(cell);
                }
            }

            return cells;
        }


        HSL rgb_to_hsl(int r, int g, int b) {
            HSL out;
            float rf = r / 255.0f;
            float gf = g / 255.0f;
            float bf = b / 255.0f;

            float luminance = 0.2126f * rf + 0.7152f * gf + 0.0722f * bf;

            out.l = luminance;

            float maxc = std::max({rf, gf, bf});
            float minc = std::min({rf, gf, bf});
            float delta = maxc - minc;

            float h = 0.0f;
            if (delta != 0.0f) {
                if (maxc == rf) {
                    h = 60.0f * std::fmod(((gf - bf) / delta), 6.0f);
                } else if (maxc == gf) {
                    h = 60.0f * (((bf - rf) / delta) + 2.0f);
                } else {
                    h = 60.0f * (((rf - gf) / delta) + 4.0f);
                }
                if (h < 0.0f) h += 360.0f;
            }

            float s = (maxc == 0.0f) ? 0.0f : (delta / maxc);

            out.h = h;
            out.s = s;

            return out;
        }
    }
}