#include <cstring>
#include <filesystem>
#include <iostream>
#include <string_view>
#include <vector>

#include "LineView.hpp"
#include "MemMapFileReader.hpp"

bool starts_with(const std::string_view str, const std::string_view prefix) {
    if (str.length() < prefix.length()) {
        return false;
    }
    return str.substr(0, prefix.length()).compare(prefix) == 0;
}

bool ends_with(const std::string_view str, const std::string_view suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.substr(str.length() - suffix.length()).compare(suffix) == 0;
}

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

    if (!std::filesystem::exists(output.parent_path())) {
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

    MemMapFileReader file(input_path.c_str());
    int msgCount = 0;
    for (auto &&line : LineView(file.view())) {
        if (starts_with(line, "msgid ")) {
            ++msgCount;
        }
    }
    std::cout << msgCount - 1 << std::endl;
    return 0;
}
