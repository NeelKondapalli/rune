#include "rune/cell.hpp"
#include "rune/converter.hpp"
#include <ostream>

namespace rune {
    namespace json {
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

        void write_cells(std::ostream& out, rune::converter::ImageBuffer& image_buffer, const std::vector<rune::Cell>& cells) {
            out << "{\n";
            out << "  \"cells\": [\n";

            for (size_t i = 0; i < cells.size(); ++i) {
                const auto& c = cells[i];

                out << "    { "
                    << "\"g\": \"" << c.glyph << "\", "
                    << "\"h\": " << int(static_cast<uint8_t>(c.h * 255.0f / 360.0f)) << ", "
                    << "\"s\": " << int(static_cast<uint8_t>(c.s * 255.0f)) << ", "
                    << "\"l\": " << int(static_cast<uint8_t>(c.l * 255.0f))
                    << " }";

                if (i + 1 < cells.size())
                    out << ",";

                out << "\n";
            }

                out << "  ]\n";
                out << "}\n";
        }
        
        void write_manifest(std::ostream& out, const rune::converter::ImageBuffer& image_buffer, int fps, int frame_count) {
            out << "{\n";
            out << "  \"cols\": " << image_buffer.width << ",\n";
            out << "  \"rows\": " << image_buffer.height / 2 << ",\n";
            out << "  \"channels\": " << image_buffer.channels << ",\n";
            out << "  \"fps\": " << fps << ",\n";
            out << "  \"frame_count\": " << frame_count << "\n";
            out << "}\n";
        }
    }
}