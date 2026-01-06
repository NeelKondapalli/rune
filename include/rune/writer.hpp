#pragma once
#include <ostream>
#include "rune/converter.hpp"
#include "rune/cell.hpp"
#include <zlib.h>

namespace rune {

    namespace writer {
        void write_cells(
            std::ostream& out,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        );

        void write_html(
            std::ostream& out,
            const std::string& html
        );

        void write_cells_gzip(
            gzFile gz,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        );

        void write_manifest(
            std::ostream& out, 
            const rune::converter::ImageBuffer& image_buffer,
            const std::string& type,
            int fps,
            int frame_count
        );
    }

}


