#pragma once
#ifndef FILE_WRAPPER_H
#define FILE_WRAPPER_H

#if defined(_WIN32)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

class FileWrapper {
  public:
    FileWrapper() = default;
    ~FileWrapper();

    void open(const char *path);
    void close();

#if defined(_WIN32)
    bool isOpen() const noexcept {
        return mHandle != INVALID_HANDLE_VALUE;
    }

    HANDLE handle() const noexcept {
        return mHandle;
    }
#else
    bool isOpen() const noexcept {
        return mFD != mInvalidFD;
    }

    int fd() const noexcept {
        return mFD;
    }
#endif

  private:
#if defined(_WIN32)
    HANDLE mHandle = INVALID_HANDLE_VALUE;
#else
    static constexpr int mInvalidFD = -1;
    int mFD = mInvalidFD;
#endif
};

#endif
