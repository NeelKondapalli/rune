#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define ZLIB_VERSION "1.3.1"
#define ZLIB_VERNUM 0x1310
#include "stb/stb_image.h"
#include "stb/stb_image_resize.h"
#include "rune/ramp.hpp"
#include "rune/converter.hpp"
#include "rune/writer.hpp"

namespace rune {
    namespace converter {
        // Converts a video file to ASCII format by extracting frames and processing each one
        void convert_video_to_ascii(const std::string& filename, int target_width, int target_fps, const std::string& output_folder, const rune::Ramp& ramp, float threshold) {
            // Create temporary directory for extracted video frames
            std::filesystem::path out_dir = "tmp";
            std::filesystem::create_directories(out_dir);

            // Clean out any existing frames from previous runs
            for (auto& entry : std::filesystem::directory_iterator(out_dir)) {
                std::filesystem::remove_all(entry.path());
            }

            // Use ffmpeg to extract video frames at target FPS
            std::string cmd =
                "ffmpeg -y -i \"" + filename + "\" "
                "-vf fps=" + std::to_string(target_fps) + " "
                + out_dir.string() + "/frame_%05d.jpg";

                int ret = std::system(cmd.c_str());
                if (ret != 0) {
                    throw std::runtime_error("ffmpeg failed");
            }

            std::filesystem::create_directories(output_folder);
            for (auto& entry : std::filesystem::directory_iterator(output_folder)) {
                std::filesystem::remove_all(entry.path());
            }

            std::ofstream manifest_out(output_folder + "/manifest.json");
            if (!manifest_out) {
                std::cerr << "failed to open output file\n";
                return;
            }

            // gather all frames in the output directory
            std::vector<std::filesystem::path> frames;

            for (const auto& entry : std::filesystem::directory_iterator(out_dir)) {
                frames.push_back(entry.path());
            }

            std::sort(frames.begin(), frames.end());

            int frame_count = frames.size();

            int counter = 0;

            bool manifest_written = false;

            std::string filename_base = output_folder + "/" + "frames";

            std::string filename_jsonl_gzip = filename_base + ".jsonl.gz";
            gzFile gz = gzopen(filename_jsonl_gzip.c_str(), "wb");

            if (!gz) {
                std::cerr << "failed to open gzip file\n";
                return;
            }

            std::string filename_jsonl = filename_base + ".jsonl";
            std::ofstream j_data_out(filename_jsonl, std::ios::out | std::ios::app);
            if (!j_data_out) {
                std::cerr << "failed to open output file\n";
                return;
            }

            std::string filename_html = filename_base + ".txt";
            std::ofstream h_data_out(filename_html, std::ios::out | std::ios::app);
            if (!h_data_out) {
                std::cerr << "failed to open output file\n";
                return;
            }

            for (auto& frame : frames) {
                AsciiFrame ascii_frame = convert_frame_to_ascii(frame.string(), target_width, ramp, threshold);

                add_html(ascii_frame);

                if (!manifest_written) {
                    const std::string type = "video";
                    writer::write_manifest(manifest_out, ascii_frame.image_buffer, type, target_fps, frame_count);
                    manifest_written = true;
                }

                writer::write_cells(j_data_out, ascii_frame.image_buffer, ascii_frame.cells);
                writer::write_cells_gzip(gz, ascii_frame.image_buffer, ascii_frame.cells);
                writer::write_html(h_data_out, ascii_frame.html);

                counter++;
                print_progress(counter, frame_count);
            }

            gzclose(gz);

        }

        void convert_image_to_ascii(const std::string& filename, int target_width, const std::string& output_folder, const rune::Ramp& ramp, float threshold) {
            std::filesystem::create_directories(output_folder);
            for (auto& entry : std::filesystem::directory_iterator(output_folder)) {
                std::filesystem::remove_all(entry.path());
            }

            std::ofstream manifest_out(output_folder + "/manifest.json");
            if (!manifest_out) {
                std::cerr << "failed to open output file\n";
                return;
            }

            std::string filename_base = output_folder + "/" + "frame";

            std::string filename_jsonl = filename_base + ".jsonl";
            std::ofstream j_data_out(filename_jsonl, std::ios::out | std::ios::app);
            if (!j_data_out) {
                std::cerr << "failed to open output file\n";
                return;
            }

            std::string filename_jsonl_gzip = filename_base + ".jsonl.gz";
            gzFile gz = gzopen(filename_jsonl_gzip.c_str(), "wb");

            if (!gz) {
                std::cerr << "failed to open gzip file\n";
                return;
            }

            std::string filename_html = filename_base + ".txt";
            std::ofstream h_data_out(filename_html, std::ios::out | std::ios::app);
            if (!h_data_out) {
                std::cerr << "failed to open output file\n";
                return;
            }

            AsciiFrame ascii_frame = convert_frame_to_ascii(filename, target_width, ramp, threshold);

            add_html(ascii_frame);

            const std::string type = "image";

            writer::write_manifest(manifest_out, ascii_frame.image_buffer, type, 0, 1);

            writer::write_cells(j_data_out, ascii_frame.image_buffer, ascii_frame.cells);
            writer::write_cells_gzip(gz, ascii_frame.image_buffer, ascii_frame.cells);
            writer::write_html(h_data_out, ascii_frame.html);

            gzclose(gz);
        }


        void add_html(AsciiFrame& ascii_frame) {
            const int width = ascii_frame.image_buffer.width;
            const auto& cells = ascii_frame.cells;

            std::string html;
            html.reserve(cells.size() * 6);

            std::string lastGlyph = "";
            int lastH = -1, lastS = -1, lastL = -1;
            std::string run;

            auto flush_run = [&]() {
                if (run.empty()) return;

                html += "<span style=\"color:hsl(";
                html += std::to_string(lastH);
                html += ",";
                html += std::to_string(lastS);
                html += "%,";
                html += std::to_string(lastL);
                html += "%)\">";
                html += run;
                html += "</span>";

                run.clear();
            };

            for (size_t i = 0; i < cells.size(); ++i) {


                if (i != 0 && i % width == 0) {
                    flush_run();
                    html += "\\n";
                    lastGlyph = "";
                    lastH = lastS = lastL = -1;
                }

                const rune::Cell& cell = cells[i];

                const std::string& glyph = cell.glyph;
                int h = static_cast<int>(cell.h);        // degrees
                int s = static_cast<int>(cell.s * 100);  // %
                int l = static_cast<int>(cell.l * 100);  // %


                if (lastGlyph.empty()) {
                    lastGlyph = glyph;
                    lastH = h;
                    lastS = s;
                    lastL = l;
                    run += glyph;
                    continue;
                }


                if (glyph == lastGlyph && h == lastH && s == lastS && l == lastL) {
                    run += glyph;
                    continue;
                }

                flush_run();
                lastGlyph = glyph;
                lastH = h;
                lastS = s;
                lastL = l;
                run += glyph;
            }


            flush_run();

            ascii_frame.html = std::move(html);
        }


        void print_progress(int counter, int frame_count) {
            const int bar_width = 40;
            float progress = static_cast<float>(counter) / frame_count;
            int filled = static_cast<int>(bar_width * progress);

            std::cout << "\r[";
            for (int i = 0; i < bar_width; ++i) {
                if (i < filled) std::cout << "█";
                else std::cout << "-";
            }

            std::cout << "] "
                      << std::setw(3) << static_cast<int>(progress * 100) << "% "
                      << "(" << counter << "/" << frame_count << ")"
                      << std::flush;
        }


        AsciiFrame convert_frame_to_ascii(const std::string& filename, int target_width, const rune::Ramp& ramp, float threshold) {
            AsciiFrame ascii_frame;
            ImageBuffer image_buffer = load_image_pixels(filename);
            ImageBuffer resized_image_buffer = resize_image_pixels(image_buffer, target_width);
            std::vector<rune::Cell> cells = pixels_to_cells(resized_image_buffer, ramp, threshold);
            ascii_frame.image_buffer = resized_image_buffer;
            ascii_frame.cells = cells;
            return ascii_frame;
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

        std::vector<rune::Cell> pixels_to_cells(const ImageBuffer& image_buffer, const rune::Ramp& ramp, float threshold) {

            std::vector<rune::Cell> cells;

            // Parse UTF-8 characters from ramp into a vector
            std::vector<std::string> ramp_chars;
            size_t i = 0;
            while (i < ramp.chars.size()) {
                unsigned char c = ramp.chars[i];
                int char_len = 1;
                
                // Determine UTF-8 character length
                if ((c & 0x80) == 0) {
                    char_len = 1; // ASCII
                } else if ((c & 0xE0) == 0xC0) {
                    char_len = 2; // 2-byte UTF-8
                } else if ((c & 0xF0) == 0xE0) {
                    char_len = 3; // 3-byte UTF-8
                } else if ((c & 0xF8) == 0xF0) {
                    char_len = 4; // 4-byte UTF-8
                }
                
                ramp_chars.push_back(std::string(ramp.chars.substr(i, char_len)));
                i += char_len;
            }

            for (int y = 0; y < image_buffer.height; y+=2) {
                for (int x = 0; x < image_buffer.width; x++) {
                    rune::Cell cell;
                    int idx = (y * image_buffer.width + x) * image_buffer.channels;
                    int r = image_buffer.pixels[idx];
                    int g = image_buffer.pixels[idx + 1];
                    int b = image_buffer.pixels[idx + 2];

                    HSL hsl = rgb_to_hsl(r, g, b);

                    // Apply threshold: if lightness > threshold, set to white (1.0), else keep original
                    float adjusted_l = (hsl.l > threshold) ? 1.0f : hsl.l;

                    float t = std::clamp(adjusted_l, 0.0f, 1.0f);

                    int ascii_index = static_cast<int>(std::round(t * (ramp_chars.size() - 1)));

                    cell.glyph = ramp_chars[ascii_index];
                    cell.h = hsl.h; // degrees
                    cell.s = hsl.s; // ratio
                    cell.l = adjusted_l; // ratio (with threshold applied)
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
