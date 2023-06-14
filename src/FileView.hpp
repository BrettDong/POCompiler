#pragma once

#include <string>
#include <string_view>

class FileView {
  public:
    explicit FileView(const std::string &path);
    virtual ~FileView();
    std::string_view view() const noexcept;

  private:
    char *mPtr;
    std::size_t mSize;
};
