#include "FileWrapper.hpp"

#include <fmt/format.h>

#if !defined(_WIN32)

#include <fcntl.h>
#include <string>
#include <unistd.h>

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

FileWrapper::~FileWrapper() {
    close();
}

#endif