#include "rune/cell.hpp"
#include "rune/converter.hpp"
#include <ostream>

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

                out << R"({"g":")" << c.glyph << R"(","h":)" << int(static_cast<uint8_t>(c.h * 255.0f / 360.0f)) << R"(,"s":)" << int(static_cast<uint8_t>(c.s * 255.0f)) << R"(,"l":)" << int(static_cast<uint8_t>(c.l * 255.0f)) << R"(})";

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
                gzwrite(gz, &c.glyph, 1);
                write(R"(","h":)");
                write(std::to_string(static_cast<uint8_t>(c.h * 255.0f / 360.0f))); // h is degress -> find ratio / 360 and norm to 255
                write(R"(,"s":)");
                write(std::to_string(static_cast<uint8_t>(c.s * 255.0f))); // s is a ratio -> norm to 255
                write(R"(,"l":)");
                write(std::to_string(static_cast<uint8_t>(c.l * 255.0f))); // l is a ratio -> norm to 255
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
            int fps, 
            int frame_count
        ) {
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