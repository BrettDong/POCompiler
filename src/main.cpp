#include "POParser.hpp"

int main(int argc, char **argv) {
    POParser parser;
    parser.parse(argv[1]);
    parser.compile(argv[2]);
    return 0;
}