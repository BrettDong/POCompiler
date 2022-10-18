#include "GenericFileReader.hpp"

#include <fstream>

GenericFileReader::GenericFileReader(const char *path) : GenericFileReader() {
    GenericFileReader::open(path);
}

void GenericFileReader::open(const char *path) {
    std::ifstream fin(path, std::ios::binary);
    mBuffer = std::string((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
}

std::string_view GenericFileReader::view() const {
    return mBuffer;
}