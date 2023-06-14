#pragma once
#include <cstring>
#include <string_view>

class LineView {
  public:
    explicit LineView(std::string_view data) : mData(data) {}
    ~LineView() = default;

    class sentinel_iterator {};

    class iterator {
      public:
        explicit iterator(const LineView &lineView) {
            if (lineView.mData.empty()) {
                mEnded = true;
            } else {
                pBegin = lineView.mData.data();
                pEnd = pBegin + lineView.mData.size();
                mEnded = false;
                findLineBreak();
            }
        }

        std::string_view operator*() const noexcept {
            return {pBegin, static_cast<std::size_t>(pLineBreak - pBegin)};
        }

        iterator &operator++() noexcept {
            if (pLineBreak == pEnd) {
                mEnded = true;
            } else {
                pBegin = pLineBreak + 1;
                findLineBreak();
            }
            return *this;
        }

        friend bool operator==(const iterator &it, const sentinel_iterator &) noexcept {
            return it.ended();
        }

        friend bool operator!=(const iterator &it, const sentinel_iterator &) noexcept {
            return !it.ended();
        }

      private:
        void findLineBreak() {
            pLineBreak = static_cast<const char *>(std::memchr(pBegin, '\n', pEnd - pBegin));
            if (pLineBreak == nullptr) {
                pLineBreak = pEnd;
            }
        }

        bool ended() const noexcept {
            return mEnded;
        }

        const char *pBegin, *pEnd, *pLineBreak;
        bool mEnded;
    };

    iterator begin() const {
        return iterator(*this);
    }

    sentinel_iterator end() const noexcept {
        return {};
    }

  private:
    std::string_view mData;
};
