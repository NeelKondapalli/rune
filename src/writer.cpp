#include "rune/cell.hpp"
#include "rune/converter.hpp"
#include <ostream>
#include <iomanip>
#include <cstdint>
#include <sstream>

namespace rune {
    namespace writer {
        // void write_json(std::ostream& out, rune::converter::ImageBuffer& image_buffer, const std::vector<rune::Cell>& cells) {
        //     out << "{\n";
        //     out << "  \"cols\": " << image_buffer.width << ",\n";
        //     out << "  \"rows\": " << image_buffer.height / 2 << ",\n";
        //     out << "  \"channels\": " << image_buffer.channels << ",\n";
        //     out << "  \"cells\": [\n";

        //     for (size_t i = 0; i < cells.size(); ++i) {
        //         const auto& c = cells[i];

        //         out << "    { "
        //             << "\"g\": \"" << c.glyph << "\", "
        //             << "\"h\": " << int(static_cast<uint8_t>(c.h * 255.0f / 360.0f)) << ", "
        //             << "\"s\": " << int(static_cast<uint8_t>(c.s * 255.0f)) << ", "
        //             << "\"l\": " << int(static_cast<uint8_t>(c.l * 255.0f))
        //             << " }";

        //         if (i + 1 < cells.size())
        //             out << ",";

        //         out << "\n";
        //     }

        //         out << "  ]\n";
        //         out << "}\n";
        //     }

        // void write_cells(std::ostream& out, rune::converter::ImageBuffer& image_buffer, const std::vector<rune::Cell>& cells) {
        //     out << "{\n";
        //     out << "  \"cells\": [\n";

        //     for (size_t i = 0; i < cells.size(); ++i) {
        //         const auto& c = cells[i];

        //         out << "    { "
        //             << "\"g\": \"" << c.glyph << "\", "
        //             << "\"h\": " << int(static_cast<uint8_t>(c.h * 255.0f / 360.0f)) << ", "
        //             << "\"s\": " << int(static_cast<uint8_t>(c.s * 255.0f)) << ", "
        //             << "\"l\": " << int(static_cast<uint8_t>(c.l * 255.0f))
        //             << " }";

        //         if (i + 1 < cells.size())
        //             out << ",";

        //         out << "\n";
        //     }

        //         out << "  ]\n";
        //         out << "}\n";
        // }

        void write_cells(
            std::ostream& out,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        ) {

            out << R"({"cells":[)";

            for (size_t i = 0; i < cells.size(); ++i) {
                const auto& c = cells[i];

                out << R"({"g":")";

                // Escape special JSON characters and encode UTF-8 as \uXXXX
                if (c.glyph == "\\") {
                    out << "\\\\";
                } else if (c.glyph == "\"") {
                    out << "\\\"";
                } else if (c.glyph == "\b") {
                    out << "\\b";
                } else if (c.glyph == "\f") {
                    out << "\\f";
                } else if (c.glyph == "\n") {
                    out << "\\n";
                } else if (c.glyph == "\r") {
                    out << "\\r";
                } else if (c.glyph == "\t") {
                    out << "\\t";
                } else if (c.glyph.size() == 1 && static_cast<unsigned char>(c.glyph[0]) < 128) {
                    // ASCII character - output directly
                    out << c.glyph;
                } else {
                    // Multi-byte UTF-8 - convert to Unicode code point and escape as \uXXXX
                    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(c.glyph.data());
                    uint32_t codepoint = 0;
                    
                    if ((bytes[0] & 0x80) == 0) {
                        codepoint = bytes[0];
                    } else if ((bytes[0] & 0xE0) == 0xC0 && c.glyph.size() >= 2) {
                        codepoint = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
                    } else if ((bytes[0] & 0xF0) == 0xE0 && c.glyph.size() >= 3) {
                        codepoint = ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
                    } else if ((bytes[0] & 0xF8) == 0xF0 && c.glyph.size() >= 4) {
                        codepoint = ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
                    }
                    
                    // Output as \uXXXX
                    out << "\\u" << std::hex << std::setfill('0') << std::setw(4) << codepoint << std::dec;
                }

                out << R"(","h":)" << static_cast<int>(c.h) << R"(,"s":)" << static_cast<int>(c.s * 100.0f) << R"(,"l":)" << static_cast<int>(c.l * 100.0f) << R"(})";

                if (i + 1 < cells.size()) {
                    out << ",";
                }

            }

            out << R"(]})" << "\n";
        }

        void write_html(
            std::ostream& out,
            const std::string& html
        ) {
            out << html << "\n";

        }

        void write_cells_gzip(
            gzFile gz,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        ) {
            auto write = [&](const std::string& s) {
                gzwrite(gz, s.data(), static_cast<unsigned int>(s.size()));
            };

            write(R"({"cells":[)");

            for (size_t i = 0; i < cells.size(); ++i) {
                const auto& c = cells[i];

                write(R"({"g":")");

                // Escape special JSON characters and encode UTF-8 as \uXXXX
                if (c.glyph == "\\") {
                    write("\\\\");
                } else if (c.glyph == "\"") {
                    write("\\\"");
                } else if (c.glyph == "\b") {
                    write("\\b");
                } else if (c.glyph == "\f") {
                    write("\\f");
                } else if (c.glyph == "\n") {
                    write("\\n");
                } else if (c.glyph == "\r") {
                    write("\\r");
                } else if (c.glyph == "\t") {
                    write("\\t");
                } else if (c.glyph.size() == 1 && static_cast<unsigned char>(c.glyph[0]) < 128) {
                    // ASCII character - output directly
                    write(c.glyph);
                } else {
                    // Multi-byte UTF-8 - convert to Unicode code point and escape as \uXXXX
                    const unsigned char* bytes = reinterpret_cast<const unsigned char*>(c.glyph.data());
                    uint32_t codepoint = 0;
                    
                    if ((bytes[0] & 0x80) == 0) {
                        codepoint = bytes[0];
                    } else if ((bytes[0] & 0xE0) == 0xC0 && c.glyph.size() >= 2) {
                        codepoint = ((bytes[0] & 0x1F) << 6) | (bytes[1] & 0x3F);
                    } else if ((bytes[0] & 0xF0) == 0xE0 && c.glyph.size() >= 3) {
                        codepoint = ((bytes[0] & 0x0F) << 12) | ((bytes[1] & 0x3F) << 6) | (bytes[2] & 0x3F);
                    } else if ((bytes[0] & 0xF8) == 0xF0 && c.glyph.size() >= 4) {
                        codepoint = ((bytes[0] & 0x07) << 18) | ((bytes[1] & 0x3F) << 12) | ((bytes[2] & 0x3F) << 6) | (bytes[3] & 0x3F);
                    }
                    
                    // Output as \uXXXX
                    std::stringstream ss;
                    ss << "\\u" << std::hex << std::setfill('0') << std::setw(4) << codepoint;
                    write(ss.str());
                }

                write(R"(","h":)");
                write(std::to_string(static_cast<int>(c.h))); // h is degrees (0-360)
                write(R"(,"s":)");
                write(std::to_string(static_cast<int>(c.s * 100.0f))); // s is ratio -> percentage (0-100)
                write(R"(,"l":)");
                write(std::to_string(static_cast<int>(c.l * 100.0f))); // l is ratio -> percentage (0-100)
                write("}");

                if (i + 1 < cells.size()) {
                    write(",");
                }
            }

            write("]}\n");
        }
        
        
        void write_manifest(
            std::ostream& out, 
            const rune::converter::ImageBuffer& image_buffer, 
            const std::string& type = "video",
            int fps = 0,
            int frame_count = 1
        ) {
            out << "{\n";
            out << "  \"cols\": " << image_buffer.width << ",\n";
            out << "  \"rows\": " << image_buffer.height / 2 << ",\n";
            out << "  \"channels\": " << image_buffer.channels << ",\n";
            out << "  \"type\": " << "\"" << type << "\""<< ",\n";
            out << "  \"fps\": " << fps << ",\n";
            out << "  \"frame_count\": " << frame_count << "\n";
            out << "}\n";
        }
    }
}