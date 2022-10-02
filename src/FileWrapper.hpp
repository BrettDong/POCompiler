#pragma once
#ifndef FILE_WRAPPER_H
#define FILE_WRAPPER_H

class FileWrapper {
  public:
    FileWrapper() : mFD(mInvalidFD) {}
    ~FileWrapper();

    void open(const char *path);
    void close();

    bool isOpen() const noexcept {
        return mFD != mInvalidFD;
    }

    int fd() const noexcept {
        return mFD;
    }

  private:
    static constexpr int mInvalidFD = -1;
    int mFD = mInvalidFD;
};

#endif
