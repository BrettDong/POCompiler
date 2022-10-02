#include "MemMapFileReader.hpp"

#include <fmt/core.h>

#include <string>
#include <sys/mman.h>
#include <sys/stat.h>

MemMapFileReader::MemMapFileReader(const char *path) {
    MemMapFileReader::open(path);
}

void MemMapFileReader::open(const char *path) {
    mFile.open(path);
    struct stat mStat {};
    if (fstat(mFile.fd(), &mStat) < 0) {
        throw std::runtime_error(fmt::format("fstat(): {}", strerror(errno)));
    }
    mFileSize = mStat.st_size;
    auto ptr = mmap(nullptr, mFileSize, PROT_READ, MAP_PRIVATE, mFile.fd(), 0);
    if (ptr == MAP_FAILED) {
        throw std::runtime_error(fmt::format("mmap(): {}", strerror(errno)));
    }
    mBuf = static_cast<char *>(ptr);
}

void MemMapFileReader::close() {
    if (mBuf != nullptr) {
        munmap(static_cast<void *>(mBuf), mFileSize);
    }
}

MemMapFileReader::~MemMapFileReader() {
    MemMapFileReader::close();
}

std::string_view MemMapFileReader::view() const {
    return {mBuf, mFileSize};
}
