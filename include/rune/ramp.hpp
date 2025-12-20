#pragma once
#include <string_view>

namespace rune {

struct Ramp {
    std::string_view chars;
};

    namespace ramps {

        constexpr Ramp SIMPLE   { " .:-=+*#%@" };
        constexpr Ramp DENSE    { " .'`^\",:;Il!i><~+_-?][}{1)(|\\/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$" };
        constexpr Ramp BLOCKS   { " ░▒▓█" };

    } // namespace ramps
} // namespace rune
