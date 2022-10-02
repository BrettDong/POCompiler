#include "MemMappedFile.hpp"

#include <fmt/core.h>

#include <string>
#include <sys/mman.h>
#include <sys/stat.h>

MemMappedFile::MemMappedFile(const char *path) : mFile(path) {
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

MemMappedFile::~MemMappedFile() {
    if (mBuf != nullptr) {
        munmap(static_cast<void *>(mBuf), mFileSize);
    }
}
