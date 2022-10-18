#pragma once
#ifndef LINE_VIEW_HPP
#define LINE_VIEW_HPP

#include <cstring>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>

class LineView {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string_view;

    class sentinel_iterator {};

    class iterator {
      public:
        iterator() : pBegin(nullptr), pEnd(nullptr), pSentinel(nullptr) {}

        explicit iterator(std::string_view sv, const char *begin) {
            if (sv.empty()) {
                pBegin = pEnd = pSentinel = nullptr;
            } else {
                pBegin = begin;
                pSentinel = sv.data() + sv.length();
                if (pBegin != pSentinel) {
                    locateEOL();
                } else {
                    pEnd = pSentinel;
                }
            }
        }

        std::string_view operator*() {
            return {pBegin, static_cast<std::size_t>(pEnd - pBegin)};
        }

        iterator &operator++() noexcept {
            if (pEnd != pSentinel) {
                pBegin = pEnd + 1;
                locateEOL();
            } else {
                pBegin = pEnd = pSentinel;
            }
            return *this;
        }

        friend bool operator==(const iterator &lhs, const sentinel_iterator &) noexcept {
            return lhs.pBegin == lhs.pSentinel;
        }

        friend bool operator!=(const iterator &lhs, const sentinel_iterator &) noexcept {
            return lhs.pBegin != lhs.pSentinel;
        }

      private:
        void locateEOL() {
            auto p = static_cast<const char *>(std::memchr(pBegin, '\n', pSentinel - pBegin));
            pEnd = (p == nullptr) ? pSentinel : p;
        }

        const char *pSentinel;
        const char *pBegin;
        const char *pEnd;
    };

    explicit LineView(std::string_view sv) : mSV(sv) {}

    explicit LineView(std::string &&str) {
        mStr.emplace(std::move(str));
        mSV = *mStr;
    }

    iterator begin() const {
        if (mSV.empty()) {
            return {};
        } else {
            return iterator(mSV, mSV.data());
        }
    }

    static sentinel_iterator end() {
        const static sentinel_iterator sentinel{};
        return sentinel;
    }

  private:
    std::string_view mSV;
    std::optional<std::string> mStr;
};

#endif
