#include "POParser.hpp"
#include "FileView.hpp"
#include "LineView.hpp"
#include "MOWriter.hpp"

#include <btree/map.h>

#include <map>
#include <vector>

using namespace std::string_literals;
using namespace std::string_view_literals;

class POParser::Impl {
  private:
    std::string mCurrentKey, mCurrentVal;
    std::string *mCurrentBuf = &mCurrentKey;
    int mCurrentPluralsCount = 0;
    btree::map<std::string, std::string> mEntries;

    enum class ParserState : std::size_t {
        Comment = 0,
        MsgCtxt,
        MsgId,
        MsgIdPlural,
        MsgStr,
        MsgStrPlural0,
        MsgStrPlural1,
        MsgStrPlural2,
        MsgStrPlural3,
        MsgStrPlural4,
        MsgStrPlural5,
        StandaloneStr
    };

    ParserState mState = ParserState::Comment;

    void fill(std::string_view input) {
        static const std::map<char, char> chars = {{'n', '\n'},  {'r', '\r'}, {'t', '\t'},
                                                   {'\\', '\\'}, {'"', '"'},  {'b', '\b'}};
        std::size_t p = 0;
        while (true) {
            std::size_t pos = input.find('\\', p);
            if (pos != std::string_view::npos) {
                if (pos + 1 == input.length()) [[unlikely]] {
                    throw std::runtime_error("Invalid char escape sequence");
                }
                mCurrentBuf->append(input.substr(p, pos - p));
                auto ch = chars.find(input[pos + 1]);
                if (ch == chars.end()) [[unlikely]] {
                    throw std::runtime_error("Invalid char escape sequence");
                } else {
                    mCurrentBuf->append(1, ch->second);
                }
                p = pos + 2;
                if (p == input.length()) {
                    break;
                }
            } else {
                mCurrentBuf->append(input.substr(p));
                break;
            }
        }
    }

    bool tryParseComment(std::string_view line) {
        if (line.starts_with('#')) {
            return true;
        }
        return false;
    }

    bool tryParseMsgCtxt(std::string_view line) {
        constexpr std::string_view prefix = "msgctxt "sv;
        constexpr ParserState state = ParserState::MsgCtxt;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentKey;
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgId(std::string_view line) {
        constexpr std::string_view prefix = "msgid "sv;
        constexpr ParserState state = ParserState::MsgId;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentKey;
            if (!mCurrentBuf->empty()) {
                mCurrentBuf->push_back('\004');
            }
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgIdPlural(std::string_view line) {
        constexpr std::string_view prefix = "msgid_plural "sv;
        constexpr ParserState state = ParserState::MsgIdPlural;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentKey;
            if (!mCurrentBuf->empty()) {
                mCurrentBuf->push_back('\0');
            }
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStr(std::string_view line) {
        constexpr std::string_view prefix = "msgstr "sv;
        constexpr ParserState state = ParserState::MsgStr;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 0;
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStrPlural0(std::string_view line) {
        constexpr std::string_view prefix = "msgstr[0] "sv;
        constexpr ParserState state = ParserState::MsgStrPlural0;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 1;
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStrPlural1(std::string_view line) {
        constexpr std::string_view prefix = "msgstr[1] "sv;
        constexpr ParserState state = ParserState::MsgStrPlural1;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 2;
            mCurrentBuf->push_back('\0');
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStrPlural2(std::string_view line) {
        constexpr std::string_view prefix = "msgstr[2] "sv;
        constexpr ParserState state = ParserState::MsgStrPlural2;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 3;
            mCurrentBuf->push_back('\0');
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStrPlural3(std::string_view line) {
        constexpr std::string_view prefix = "msgstr[3] "sv;
        constexpr ParserState state = ParserState::MsgStrPlural3;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 4;
            mCurrentBuf->push_back('\0');
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStrPlural4(std::string_view line) {
        constexpr std::string_view prefix = "msgstr[4] "sv;
        constexpr ParserState state = ParserState::MsgStrPlural4;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 5;
            mCurrentBuf->push_back('\0');
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseMsgStrPlural5(std::string_view line) {
        constexpr std::string_view prefix = "msgstr[5] "sv;
        constexpr ParserState state = ParserState::MsgStrPlural5;
        if (line.starts_with(prefix)) {
            mState = state;
            mCurrentBuf = &mCurrentVal;
            mCurrentPluralsCount = 6;
            mCurrentBuf->push_back('\0');
            fill(line.substr(prefix.length() + 1, line.length() - (prefix.length() + 2)));
            return true;
        }
        return false;
    }

    bool tryParseStandaloneStr(std::string_view line) {
        if (line.starts_with('"')) {
            mState = ParserState::StandaloneStr;
            fill(line.substr(1, line.length() - 2));
            return true;
        }
        return false;
    }

    using ParseLineFn = bool (POParser::Impl::*)(std::string_view);

    /* clang-format off */
    static inline const std::map<ParserState, std::vector<ParseLineFn>> ParseLineFunctionsMap{
        {ParserState::Comment, {&POParser::Impl::tryParseComment, &POParser::Impl::tryParseMsgId, &POParser::Impl::tryParseMsgCtxt}},
        {ParserState::MsgCtxt, {&POParser::Impl::tryParseMsgId, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgId, {&POParser::Impl::tryParseMsgStr, &POParser::Impl::tryParseMsgIdPlural, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgIdPlural, {&POParser::Impl::tryParseMsgStrPlural0, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStr, {&POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural0, {&POParser::Impl::tryParseMsgStrPlural1, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural1, {&POParser::Impl::tryParseMsgStrPlural2, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural2, {&POParser::Impl::tryParseMsgStrPlural3, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural3, {&POParser::Impl::tryParseMsgStrPlural4, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural4, {&POParser::Impl::tryParseMsgStrPlural5, &POParser::Impl::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural5, {&POParser::Impl::tryParseStandaloneStr}},
        {ParserState::StandaloneStr, {&POParser::Impl::tryParseMsgId, &POParser::Impl::tryParseMsgIdPlural, &POParser::Impl::tryParseMsgStr, &POParser::Impl::tryParseMsgStrPlural0, &POParser::Impl::tryParseMsgStrPlural1, &POParser::Impl::tryParseMsgStrPlural2, &POParser::Impl::tryParseMsgStrPlural3, &POParser::Impl::tryParseMsgStrPlural4, &POParser::Impl::tryParseMsgStrPlural5, &POParser::Impl::tryParseStandaloneStr}}
    };
    /* clang-format on */

    void commitCurrentEntry() {
        if (!mCurrentVal.empty() && (mCurrentVal.length() + 1 > mCurrentPluralsCount)) {
            mEntries.insert({std::move(mCurrentKey), std::move(mCurrentVal)});
        }
        mCurrentKey = std::string{};
        mCurrentVal = std::string{};
        mCurrentPluralsCount = 0;
        mState = ParserState::Comment;
    }

  public:
    void parse(const std::string &poPath) {
        auto fileView = FileView{poPath};
        auto lineView = LineView{fileView.view()};
        int lineNumber = 0;
        for (std::string_view line : lineView) {
            ++lineNumber;
            if (line.empty()) {
                commitCurrentEntry();
                continue;
            }
            const auto &functions = ParseLineFunctionsMap.at(mState);
            bool parsed = false;
            for (auto &&fn : functions) {
                if ((this->*fn)(line)) {
                    parsed = true;
                    break;
                }
            }
            if (!parsed) {
                throw std::runtime_error("Invalid PO at "s + std::to_string(lineNumber));
            }
        }
        commitCurrentEntry();
    }

    void compile(const std::string &moPath) {
        writeMO(mEntries, moPath.c_str());
    }
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
