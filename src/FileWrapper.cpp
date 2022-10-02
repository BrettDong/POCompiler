#include "FileWrapper.hpp"

#include <fmt/format.h>

#include <fcntl.h>
#include <string>
#include <unistd.h>

FileWrapper::FileWrapper(const char *path) {
    mFD = open(path, O_RDONLY);
    if (mFD == -1) {
        throw std::runtime_error(fmt::format("open(): {}", strerror(errno)));
    }
}

FileWrapper::~FileWrapper() {
    if (mFD != mInvalidFD) {
        close(mFD);
    }
}
