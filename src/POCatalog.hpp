#pragma once

#include <memory>
#include <string>

class POCatalog {
  private:
    class Impl;
    std::unique_ptr<Impl> mImpl;

  public:
    POCatalog();
    POCatalog(const POCatalog &) = delete;
    POCatalog(POCatalog &&) noexcept;
    ~POCatalog();
    void addTranslatedMessage(std::string &&key, std::string &&val);
    void compileToMO(const std::string &path);
};
