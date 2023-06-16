#pragma once

#include <memory>
#include <string>

class POParser {
  public:
    POParser();
    ~POParser();

    POParser(const POParser &) = delete;
    POParser(POParser &&) noexcept;

    void parse(const std::string &poPath);
    void compile(const std::string &moPath);

  private:
    class Impl;
    std::unique_ptr<Impl> mImpl;
};
