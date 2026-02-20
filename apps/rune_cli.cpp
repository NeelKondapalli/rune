#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>

#include "rune/converter.hpp"
#include "rune/ramp.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage:\n"
                  << "  rune_cli --image <filename> [--width N] [--ramp simple|dense|blocks|dot|dot2] [--custom-ramp <string>] [--threshold 0-1] [--out folder]\n"
                  << "  rune_cli --video <filename> [--width N] [--target-fps N] [--ramp simple|dense|blocks|dot|dot2] [--custom-ramp <filename>] [--threshold 0-1] [--out folder]\n";
        return 1;
    }

    std::string input = argv[2];
    std::string output;
    int width = 120;
    int target_fps = 8;
    std::string ramp_name = "simple";
    rune::Ramp custom_ramp_obj;
    std::string custom_ramp;
    float threshold = 1.0f;  // Default 1.0 = no filtering (all colors survive)

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--width" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        }
        else if (arg == "--target-fps" && i + 1 < argc) {
            target_fps = std::stoi(argv[++i]);
        }
        else if (arg == "--ramp" && i + 1 < argc) {
            ramp_name = argv[++i];
        }
        else if (arg == "--custom-ramp" && i + 1 < argc) {
            custom_ramp = argv[++i];
        }
        else if (arg == "--threshold" && i + 1 < argc) {
            threshold = std::stof(argv[++i]);
            threshold = std::clamp(threshold, 0.0f, 1.0f);
        }
        else if (arg == "--out" && i + 1 < argc) {
            output = argv[++i];
        }
        else {
            std::cerr << "unknown argument: " << arg << "\n";
            return 1;
        }
    }

    // Select the ramp based on the ramp_name
    const rune::Ramp* ramp = &rune::ramps::SIMPLE;
    if (!custom_ramp.empty()) {
        custom_ramp_obj = rune::Ramp {custom_ramp};
        ramp = &custom_ramp_obj;
    } else if (ramp_name == "simple") {
        ramp = &rune::ramps::SIMPLE;
    } else if (ramp_name == "dense") {
        ramp = &rune::ramps::DENSE;
    } else if (ramp_name == "blocks") {
        ramp = &rune::ramps::BLOCKS;
    } else if (ramp_name == "dot") {
        ramp = &rune::ramps::DOT;
    } else if (ramp_name == "line") {
        ramp = &rune::ramps::LINE;
    } else {
        std::cerr << "unknown ramp: " << ramp_name << "\n";
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "--image") {
        rune::converter::convert_image_to_ascii(input, width, output, *ramp, threshold);
    } else if (mode == "--video") {
        rune::converter::convert_video_to_ascii(input, width, target_fps, output, *ramp, threshold);
    } else {
        std::cerr << "unknown mode: " << mode << "\n";
        return 1;
    }

    std::cout << "\n\nconversion complete. thank you for using ᛚune\n" << std::endl;
    return 0;
}
