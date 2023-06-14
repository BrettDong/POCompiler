#include "POParser.hpp"
#include "FileView.hpp"
#include "LineView.hpp"
#include <fc_btree.h>

class POParser::Impl {
  public:
    void parse(const std::string &poPath) {
        auto fileView = FileView{poPath};
    }

    void compile(const std::string &moPath) {}

  private:
    frozenca::BTreeMap<std::string, std::string> mEntries;
};

POParser::POParser() {
    mImpl = std::make_unique<Impl>();
}

POParser::POParser(POParser &&) noexcept = default;

POParser::~POParser() = default;

void POParser::parse(const std::string &poPath) {
    mImpl->parse(poPath);
}

void POParser::compile(const std::string &moPath) {
    mImpl->compile(moPath);
}
