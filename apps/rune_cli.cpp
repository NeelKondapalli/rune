#include <iostream>
#include <fstream>
#include <string>

#include "rune/converter.hpp"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage:\n"
                  << "  rune_cli --image <filename> [--width N] [--out file]\n"
                  << "  rune_cli --video <filename> [--width N] [--target-fps N] [--out folder]\n";
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "--image") {
        mode = "image";
    } else if (mode == "--video") {
        mode = "video";
    } else {
        std::cerr << "unknown mode: " << argv[1] << "\n";
        return 1;
    }

    std::string input = argv[2];
    std::string output;
    int width = 120;
    int target_fps = 8;

    for (int i = 3; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--width" && i + 1 < argc) {
            width = std::stoi(argv[++i]);
        }
        else if (arg == "--target-fps" && i + 1 < argc) {
            target_fps = std::stoi(argv[++i]);
        }

        else if (arg == "--out" && i + 1 < argc) {
            output = argv[++i];
        }
        else {
            std::cerr << "unknown argument: " << arg << "\n";
            return 1;
        }
    }

    // if (mode == "image") {
    //     if (output.empty()) {
    //         rune::converter::convert_image_to_ascii(input, width, std::cout);
    //     } else {
    //         std::ofstream out(output);
    //         if (!out) {
    //             std::cerr << "failed to open output file\n";
    //             return 1;
    //         }
    //         rune::converter::convert_image_to_ascii(input, width, out);
    //     }
    // }
    if (mode == "video") {
        rune::converter::convert_video_to_ascii(input, width, target_fps, output);
    } else {
        std::cerr << "unknown mode: " << mode << "\n";
        return 1;
    }

    std::cout << "\nconversion complete. thank you for using ᛚune\n" << std::endl;
    return 0;
}
