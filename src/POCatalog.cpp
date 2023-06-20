#include "POCatalog.hpp"
#include "MOWriter.hpp"

#include <btree/map.h>

class POCatalog::Impl {
  private:
    btree::map<std::string, std::string> mEntries;

    void removePOTCreationDate() {
        try {
            auto &it = mEntries.at("");
            auto p1 = it.find("POT-Creation-Date: ");
            if (p1 == std::string::npos) {
                return;
            }
            auto p2 = it.find('\n', p1);
            if (p2 == std::string::npos || p2 + 1 == it.length()) {
                it = it.substr(0, p1);
                return;
            }
            it = it.substr(0, p1) + it.substr(p2 + 1);
        } catch (std::out_of_range &) {
            return;
        }
    }

  public:
    void addTranslatedMessage(std::string &&key, std::string &&val) {
        mEntries.insert({std::move(key), std::move(val)});
    }

    void compileToMO(const std::string &path) {
        removePOTCreationDate();
        writeMO(mEntries, path.c_str());
    }
};

POCatalog::POCatalog() {
    mImpl = std::make_unique<Impl>();
}

POCatalog::POCatalog(POCatalog &&) noexcept = default;

POCatalog::~POCatalog() = default;

void POCatalog::addTranslatedMessage(std::string &&key, std::string &&val) {
    mImpl->addTranslatedMessage(std::move(key), std::move(val));
}

void POCatalog::compileToMO(const std::string &path) {
    mImpl->compileToMO(path);
}