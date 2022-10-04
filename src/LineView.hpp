#pragma once
#ifndef LINE_VIEW_HPP
#define LINE_VIEW_HPP

#include <cstring>
#include <iterator>
#include <string_view>

class LineView {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string_view;

    class sentinel_iterator {};

    class iterator {
      public:
        explicit iterator(std::string_view sv, const char *begin)
            : mSV(sv), pBegin(begin), pEnd(begin) {
            if (begin != mSV.end()) {
                locateEOL();
            } else {
                pBegin = pEnd = nullptr;
            }
        }

        std::string_view operator*() {
            return {pBegin, static_cast<std::size_t>(pEnd - pBegin)};
        }

        iterator &operator++() noexcept {
            if (pEnd != mSV.end()) {
                pBegin = pEnd + 1;
                locateEOL();
            } else {
                pBegin = pEnd = nullptr;
            }
            return *this;
        }

        friend bool operator==(const iterator &lhs, const sentinel_iterator &sentinel) noexcept {
            return lhs.pEnd == nullptr;
        }

        friend bool operator!=(const iterator &lhs, const sentinel_iterator &sentinel) noexcept {
            return lhs.pEnd != nullptr;
        }

      private:
        void locateEOL() {
            const char *p =
                static_cast<const char *>(std::memchr(pBegin, '\n', mSV.end() - pBegin));
            pEnd = (p == nullptr) ? mSV.end() : p;
        }

        const char *pBegin;
        const char *pEnd;
        std::string_view mSV;
    };

    explicit LineView(std::string_view sv) : mSV(sv) {}

    iterator begin() const {
        return iterator(mSV, mSV.begin());
    }

    static sentinel_iterator end() {
        const static sentinel_iterator sentinel{};
        return sentinel;
    }

  private:
    std::string_view mSV;
};

#endif
