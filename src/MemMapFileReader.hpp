#pragma once
#ifndef MEM_MAP_FILE_READER_HPP
#define MEM_MAP_FILE_READER_HPP

#include "FileReader.hpp"
#include "FileWrapper.hpp"

#include <cstddef>
#include <iterator>

class MemMapFileReader : public FileReader {
  public:
    MemMapFileReader() : mFileSize(0), mBuf(nullptr) {}
    explicit MemMapFileReader(const char *path);
    ~MemMapFileReader();

    void open(const char *path) override;
    std::string_view view() const override;
    void close() override;

  private:
    std::size_t mFileSize;
    char *mBuf = nullptr;
    FileWrapper mFile;
};

#endif