#include "POParser.hpp"
#include "FileView.hpp"
#include "LineView.hpp"

#include <map>
#include <vector>

using namespace std::string_literals;
using namespace std::string_view_literals;

class POParser {
  private:
    POCatalog &mCatalog;
    std::string mCurrentKey, mCurrentVal;
    std::string *mCurrentBuf = &mCurrentKey;
    int mCurrentPluralsCount = 0;

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

    using ParseLineFn = bool (POParser::*)(std::string_view);

    /* clang-format off */
    static inline const std::map<ParserState, std::vector<ParseLineFn>> ParseLineFunctionsMap{
        {ParserState::Comment, {&POParser::tryParseComment, &POParser::tryParseMsgId, &POParser::tryParseMsgCtxt}},
        {ParserState::MsgCtxt, {&POParser::tryParseMsgId, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgId, {&POParser::tryParseMsgStr, &POParser::tryParseMsgIdPlural, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgIdPlural, {&POParser::tryParseMsgStrPlural0, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgStr, {&POParser::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural0, {&POParser::tryParseMsgStrPlural1, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural1, {&POParser::tryParseMsgStrPlural2, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural2, {&POParser::tryParseMsgStrPlural3, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural3, {&POParser::tryParseMsgStrPlural4, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural4, {&POParser::tryParseMsgStrPlural5, &POParser::tryParseStandaloneStr}},
        {ParserState::MsgStrPlural5, {&POParser::tryParseStandaloneStr}},
        {ParserState::StandaloneStr, {&POParser::tryParseMsgId, &POParser::tryParseMsgIdPlural, &POParser::tryParseMsgStr, &POParser::tryParseMsgStrPlural0, &POParser::tryParseMsgStrPlural1, &POParser::tryParseMsgStrPlural2, &POParser::tryParseMsgStrPlural3, &POParser::tryParseMsgStrPlural4, &POParser::tryParseMsgStrPlural5, &POParser::tryParseStandaloneStr}}
    };
    /* clang-format on */

    void commitCurrentEntry() {
        if (!mCurrentVal.empty() && (mCurrentVal.length() + 1 > mCurrentPluralsCount)) {
            mCatalog.addTranslatedMessage(std::move(mCurrentKey), std::move(mCurrentVal));
        }
        mCurrentKey.clear();
        mCurrentVal.clear();
        mCurrentPluralsCount = 0;
        mState = ParserState::Comment;
    }

  public:
    explicit POParser(POCatalog &catalog) : mCatalog(catalog) {}

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
};

POCatalog parsePOFile(const std::string &path) {
    POCatalog catalog;
    POParser parser{catalog};
    parser.parse(path);
    return catalog;
}
