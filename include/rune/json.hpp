#pragma once
#include <ostream>
#include "rune/converter.hpp"
#include "rune/cell.hpp"

namespace rune {

    namespace json {
        void write_json(
            std::ostream& out,
            rune::converter::ImageBuffer& image_buffer,
            const std::vector<rune::Cell>& cells
        );
    }

}