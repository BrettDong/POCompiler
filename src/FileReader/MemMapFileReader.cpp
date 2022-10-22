#include "MemMapFileReader.hpp"
#include "ErrorUtil.hpp"

#include <fmt/core.h>

#include <stdexcept>

#if defined(_WIN32)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include <sys/mman.h>
#include <sys/stat.h>
#endif

MemMapFileReader::MemMapFileReader(const char *path) {
    MemMapFileReader::open(path);
}

MemMapFileReader::~MemMapFileReader() {
    MemMapFileReader::close();
}

std::string_view MemMapFileReader::view() const {
    return {mBuf, mFileSize};
}

#if defined(_WIN32)
void MemMapFileReader::open(const char *path) {
    mFile.open(path);
    mMapping = CreateFileMapping(mFile.handle(), nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (mMapping == INVALID_HANDLE_VALUE) {
        throw std::runtime_error(fmt::format("CreateFileMapping(): {}", getSystemError()));
    }
    DWORD fileSizeHi = 0;
    DWORD fileSizeLo = GetFileSize(mFile.handle(), &fileSizeHi);
    mFileSize = (static_cast<std::size_t>(fileSizeHi) << 32) | fileSizeLo;
    LPVOID addr = MapViewOfFile(mMapping, FILE_MAP_READ, 0, 0, mFileSize);
    if (addr == nullptr) {
        throw std::runtime_error(fmt::format("MapViewOfFile(): {}", getSystemError()));
    }
    mBuf = static_cast<char *>(addr);
}

void MemMapFileReader::close() {
    if (mBuf != nullptr) {
        auto addr = static_cast<LPVOID>(mBuf);
        UnmapViewOfFile(addr);
        CloseHandle(mMapping);
        mMapping = INVALID_HANDLE_VALUE;
        mBuf = nullptr;
    }
}
#else
void MemMapFileReader::open(const char *path) {
    mFile.open(path);
    struct stat mStat {};
    if (fstat(mFile.fd(), &mStat) < 0) {
        throw std::runtime_error(fmt::format("fstat(): {}", getSystemError()));
    }
    mFileSize = mStat.st_size;
    auto ptr = mmap(nullptr, mFileSize, PROT_READ, MAP_PRIVATE, mFile.fd(), 0);
    if (ptr == MAP_FAILED) {
        throw std::runtime_error(fmt::format("mmap(): {}", getSystemError()));
    }
    mBuf = static_cast<char *>(ptr);
}

void MemMapFileReader::close() {
    if (mBuf != nullptr) {
        munmap(static_cast<void *>(mBuf), mFileSize);
        mBuf = nullptr;
    }
}
#endif