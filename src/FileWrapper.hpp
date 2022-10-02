#pragma once
#ifndef FILE_WRAPPER_H
#define FILE_WRAPPER_H

class FileWrapper {
  public:
    explicit FileWrapper(const char *path);
    ~FileWrapper();

    auto fd() const noexcept -> int {
        return mFD;
    }

  private:
    static constexpr int mInvalidFD = -1;
    int mFD = mInvalidFD;
};

#endif
