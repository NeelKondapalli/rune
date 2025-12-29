#pragma once
#include <ostream>
#include "rune/converter.hpp"
#include "rune/cell.hpp"
#include <zlib.h>

namespace rune {

    namespace json {
        void write_cells(
            std::ostream& out,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        );

        void write_cells_gzip(
            gzFile gz,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        );

        void write_manifest(
            std::ostream& out, 
            const rune::converter::ImageBuffer& image_buffer,
            int fps,
            int frame_count
        );
    }

}


