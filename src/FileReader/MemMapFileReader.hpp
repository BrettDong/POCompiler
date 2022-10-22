#pragma once
#ifndef MEM_MAP_FILE_READER_HPP
#define MEM_MAP_FILE_READER_HPP

#include "FileReader.hpp"
#include "FileWrapper.hpp"

#include <cstddef>

class MemMapFileReader : public FileReader {
  public:
    MemMapFileReader() = default;
    explicit MemMapFileReader(const char *path);
    ~MemMapFileReader();

    void open(const char *path) override;
    std::string_view view() const override;
    void close() override;

  private:
    std::size_t mFileSize = 0;
    char *mBuf = nullptr;
    FileWrapper mFile;
#if defined(_WIN32)
    HANDLE mMapping = INVALID_HANDLE_VALUE;
#endif
};

#endif