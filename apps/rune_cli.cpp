#include <iostream>
#include <fstream>
#include <string>

#include "rune/converter.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage:\n"
                  << "  rune_cli <image> [--width N] [--out file]\n"
                  << "  rune_cli <frame.json> --view\n";
        return 1;
    }

    std::string input = argv[1];
    std::string output;
    int width = 120;
    bool view = false;

    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--width" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        }
        else if (arg == "--out" && i + 1 < argc) {
            output = argv[++i];
        }
        else {
            std::cerr << "unknown argument: " << arg << "\n";
            return 1;
        }
    }


    if (output.empty()) {
        rune::converter::convert_image_to_ascii(input, width, std::cout);
    } else {
        std::ofstream out(output);
        if (!out) {
            std::cerr << "failed to open output file\n";
            return 1;
        }
        rune::converter::convert_image_to_ascii(input, width, out);
    }

    std::cout << "conversion complete. thank you for using rune\n";
    return 0;
}
