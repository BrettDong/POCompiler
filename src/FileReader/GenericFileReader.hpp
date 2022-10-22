#pragma once
#ifndef GENERIC_FILE_READER_HPP
#define GENERIC_FILE_READER_HPP

#include "FileReader.hpp"

#include <string>

class GenericFileReader : public FileReader {
  public:
    GenericFileReader() = default;
    explicit GenericFileReader(const char *path);
    void open(const char *path) override;
    std::string_view view() const override;
    void close() override;

  protected:
    std::string mBuffer;
};

#endif