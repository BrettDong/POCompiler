#include "FileWrapper.hpp"
#include "ErrorUtil.hpp"

#include <fmt/format.h>
#include <string>

#if !defined(_WIN32)
#include <fcntl.h>
#include <unistd.h>
#endif

#if defined(_WIN32)
void FileWrapper::open(const char *path) {
    mHandle = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                          FILE_ATTRIBUTE_NORMAL, nullptr);
    if (mHandle == INVALID_HANDLE_VALUE) {

        throw std::runtime_error(fmt::format("CreateFileA(): {}", getSystemError()));
    }
}

void FileWrapper::close() {
    if (mHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(mHandle);
        mHandle = INVALID_HANDLE_VALUE;
    }
}
#else
void FileWrapper::open(const char *path) {
    mFD = ::open(path, O_RDONLY);
    if (mFD == -1) {
        throw std::runtime_error(fmt::format("open(): {}", strerror(errno)));
    }
}

void FileWrapper::close() {
    if (isOpen()) {
        ::close(mFD);
        mFD = mInvalidFD;
    }
}
#endif

FileWrapper::~FileWrapper() {
    close();
}
