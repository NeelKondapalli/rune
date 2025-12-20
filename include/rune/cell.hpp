#pragma once
#include <cstdint>

namespace rune {

    struct Cell {
        char  glyph;  
        float h;      // hue   [0–255]  → map to [0–360] in renderer
        float s;      // sat   [0–255]
        float l;      // lum [0–255]
    };

} // namespace rune
