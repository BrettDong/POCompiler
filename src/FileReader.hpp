#pragma once
#ifndef FILE_READER_HPP
#define FILE_READER_HPP

#include <string_view>

class FileReader {
  public:
    virtual ~FileReader() = default;
    virtual void open(const char *path) = 0;
    virtual std::string_view view() const = 0;
    virtual void close() = 0;
};

#endif
