#pragma once
#ifndef MEM_MAPPED_FILE_HPP
#define MEM_MAPPED_FILE_HPP

#include "FileWrapper.hpp"

#include <cstddef>
#include <iterator>

class MemMappedFile {
  public:
    explicit MemMappedFile(const char *path);
    ~MemMappedFile();

    auto buf() const noexcept -> char * {
        return mBuf;
    }

    auto size() const noexcept -> std::size_t {
        return mFileSize;
    }

    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = char;
    using pointer = value_type *;
    using reference = value_type &;

    class iterator {
      public:
        explicit iterator(pointer ptr) : mPtr(ptr) {}

        reference operator*() const noexcept {
            return *mPtr;
        }

        pointer operator->() const noexcept {
            return mPtr;
        }

        iterator &operator++() noexcept {
            ++mPtr;
            return *this;
        }

        friend bool operator==(const iterator &lhs, const iterator &rhs) noexcept {
            return lhs.mPtr == rhs.mPtr;
        }

        friend bool operator!=(const iterator &lhs, const iterator &rhs) noexcept {
            return lhs.mPtr != rhs.mPtr;
        }

      private:
        pointer mPtr;
    };

    iterator begin() const {
        return iterator(mBuf);
    }

    iterator end() const {
        return iterator(mBuf + mFileSize);
    }

    using const_iterator = iterator;

  private:
    std::size_t mFileSize;
    char *mBuf = nullptr;
    FileWrapper mFile;
};

#endif