#pragma once
#ifndef LINE_VIEW_HPP
#define LINE_VIEW_HPP

#include <iterator>
#include <string_view>

class LineView {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string_view;

    class sentinel_iterator {};

    class iterator {
      public:
        explicit iterator(std::string_view sv, std::string_view::const_iterator begin)
            : mSV(sv), pBegin(begin), pEnd(begin) {
            if (begin != mSV.end()) {
                locateEOL();
            } else {
                mEnded = true;
            }
        }

        std::string_view operator*() {
            return {&*pBegin, static_cast<std::size_t>(&*pEnd - &*pBegin)};
        }

        iterator &operator++() noexcept {
            pBegin = pEnd;
            if (pEnd != mSV.end()) {
                ++pBegin;
                locateEOL();
            } else {
                mEnded = true;
            }
            return *this;
        }

        friend bool operator==(const iterator &lhs, const sentinel_iterator &sentinel) noexcept {
            return lhs.mEnded;
        }

        friend bool operator!=(const iterator &lhs, const sentinel_iterator &sentinel) noexcept {
            return !lhs.mEnded;
        }

      private:
        void locateEOL() {
            pEnd = pBegin;
            while (pEnd != mSV.end() && *pEnd != '\n') {
                ++pEnd;
            }
        }

        std::string_view::const_iterator pBegin;
        std::string_view::const_iterator pEnd;
        std::string_view mSV;
        bool mEnded = false;
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
