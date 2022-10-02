#pragma once
#ifndef LINE_VIEW_HPP
#define LINE_VIEW_HPP

#include <iterator>
#include <type_traits>

template <typename T,
          std::enable_if_t<std::is_same_v<typename T::value_type, char>, void *> = nullptr>
class LineView {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = std::string_view;

    class sentinel_iterator {};

    class iterator {
      public:
        explicit iterator(const T &source, typename T::const_iterator begin)
            : mSource(source), pBegin(begin), pEnd(begin) {
            if (begin != source.end()) {
                locateEOL();
            } else {
                mEnded = true;
            }
        }

        value_type operator*() {
            return std::string_view(&*pBegin, &*pEnd - &*pBegin);
        }

        iterator &operator++() noexcept {
            pBegin = pEnd;
            if (pEnd != mSource.end()) {
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
            while (pEnd != mSource.end() && *pEnd != '\n') {
                ++pEnd;
            }
        }

        typename T::const_iterator pBegin;
        typename T::const_iterator pEnd;
        const T &mSource;
        bool mEnded = false;
    };

    explicit LineView(const T &source) : mSource(source) {}

    iterator begin() const {
        return iterator(mSource, mSource.begin());
    }

    sentinel_iterator end() const {
        const static sentinel_iterator sentinel;
        return sentinel;
    }

  private:
    const T &mSource;
};

#endif
