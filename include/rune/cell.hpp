#pragma once
#include <cstdint>
#include <string>

namespace rune {

    struct Cell {
        std::string glyph;
        float h;      // hue   [0–255]  → map to [0–360] in renderer
        float s;      // sat   [0–255]
        float l;      // lum [0–255]
    };

} // namespace rune
