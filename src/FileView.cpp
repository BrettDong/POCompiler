#include "FileView.hpp"
#include "SystemError.hpp"
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

class FileDescriptor {
  public:
    explicit FileDescriptor(const std::string &path) {
        mFD = open(path.c_str(), O_RDONLY);
        if (mFD == -1) {
            THROW_SYSTEM_ERROR();
        }
    }

    ~FileDescriptor() noexcept {
        close(mFD);
    }

    int fd() const noexcept {
        return mFD;
    }

  private:
    int mFD;
};

FileView::FileView(const std::string &path) {
    auto fd = FileDescriptor{path};
    struct stat stat {};
    if (fstat(fd.fd(), &stat) < 0) {
        THROW_SYSTEM_ERROR();
    }
    mSize = stat.st_size;
    void *p = mmap(nullptr, mSize, PROT_READ, MAP_PRIVATE, fd.fd(), 0);
    if (p == MAP_FAILED) {
        THROW_SYSTEM_ERROR();
    }
    mPtr = static_cast<char *>(p);
}

FileView::~FileView() {
    munmap(static_cast<void *>(mPtr), mSize);
}

std::string_view FileView::view() const noexcept {
    return {mPtr, mSize};
}
