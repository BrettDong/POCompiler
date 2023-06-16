#include "POParser.hpp"
#include <cstring>
#include <iostream>

int main(int argc, char **argv) {
    if (argc == 1) {
        std::cout << "Usage: POCompiler -o out.mo input.po" << std::endl;
        return 0;
    }
    std::string input_path;
    std::string output_path;
    for (int i = 1; i < argc; i++) {
        if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if (argv[i][0] == '-') {
            continue;
        } else if (std::string_view{argv[i]}.ends_with(".po")) {
            input_path = argv[i];
        }
    }
    if (input_path.empty() || output_path.empty()) {
        std::cout << "Usage: POCompiler -o out.mo input.po" << std::endl;
        return 0;
    }
    POParser parser;
    parser.parse(input_path);
    parser.compile(output_path);
    return 0;
}
