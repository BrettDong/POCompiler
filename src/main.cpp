#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>

#include "GenericFileReader.hpp"
#include "LineView.hpp"
#include "MemMapFileReader.hpp"
#include "Parser.hpp"
#include "StringUtil.hpp"
#include "Writer.hpp"

#include <fmt/format.h>

int main(int argc, char *argv[]) {
    std::string input_path;
    std::string output_path;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if (argv[i][0] == '-') {
            std::cout << "Warning: Unknown flag " << argv[i] << std::endl;
        } else if (ends_with(argv[i], ".po")) {
            input_path = argv[i];
        }
    }
    if (input_path.empty()) {
        std::cout << "Fatal: Input file path unspecified" << std::endl;
        return 1;
    }
    if (output_path.empty()) {
        std::cout << "Fatal: Output file path unspecified" << std::endl;
        return 1;
    }

    auto input = std::filesystem::path(input_path);
    auto output = std::filesystem::path(output_path);

    if (!std::filesystem::exists(input)) {
        std::cout << "Fatal: Input file \"" << input_path << "\" does not exist" << std::endl;
        return 1;
    }

    if (output.has_parent_path() && !std::filesystem::exists(output.parent_path())) {
        std::cout << "Fatal: Output path \"" << output.parent_path() << "\" does not exist"
                  << std::endl;
        return 1;
    }

    if (!input.has_extension() || input.extension() != ".po") {
        std::cout << "Fatal: Input file \"" << input_path << "\" does not seem to be a .po file"
                  << std::endl;
        return 1;
    }

    if (!output.has_extension() || output.extension() != ".mo") {
        std::cout << "Fatal: Output file \"" << input_path << "\" does not seem to be a .mo file"
                  << std::endl;
        return 1;
    }

    std::unique_ptr<FileReader> input_file;
#if defined(_WIN32)
    input_file = std::make_unique<GenericFileReader>(input_path.c_str());
#else
    input_file = std::make_unique<MemMapFileReader>(input_path.c_str());
#endif
    auto entries = Parser{}.parsePOFile(*input_file);
    writeMO(entries, output_path.c_str());
    return 0;
}
