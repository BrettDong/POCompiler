#include "Parser.hpp"
#include "FileReader/FileReader.hpp"
#include "LineView.hpp"
#include "StringUtil.hpp"

#include <algorithm>

#include <fmt/format.h>

std::string_view Parser::extractStringContent(std::string_view line, std::size_t prefix_length) {
    return line.substr(prefix_length + 1, line.length() - prefix_length - 2);
}

void Parser::clearCurrentEntry() {
    msgctxt.clear();
    msgid.clear();
    msgid_plural.clear();
    msgstr.clear();
    for (std::string &str : msgstr_plural) {
        str.clear();
    }
}

bool Parser::isSingular() const {
    return msgid_plural.empty();
}

bool Parser::isPlural() const {
    return !msgid_plural.empty();
}

bool Parser::isTranslated() const {
    if (isSingular()) {
        return !msgstr.empty();
    } else {
        return std::any_of(msgstr_plural.begin(), msgstr_plural.begin() + nPlurals,
                           [](const std::string &str) { return !str.empty(); });
    }
}

std::string Parser::originalStringRepresentation() const {
    std::string result;
    if (!msgctxt.empty()) {
        result.append(unescape(msgctxt));
        result.append(1, '\x04');
    }
    result.append(unescape(msgid));
    if (isPlural()) {
        result.append(1, '\x00');
        result.append(unescape(msgid_plural));
    }
    return result;
}

std::string Parser::translatedStringRepresentation() const {
    std::string result;
    if (!isPlural()) {
        result.append(unescape(msgstr));
    } else {
        std::size_t last;
        for (last = 0; last < nPlurals; ++last) {
            if (msgstr_plural[last].empty()) {
                break;
            }
        }
        for (std::size_t i = 0; i + 1 < last; i++) {
            result.append(unescape(msgstr_plural[i]));
            result.append(1, '\x00');
        }
        result.append(unescape(msgstr_plural[last - 1]));
    }
    return result;
}

std::string Parser::parseMetaData(const std::string &metaData) {
    std::vector<std::string> linesKept;
    for (auto &&line : LineView(metaData)) {
        if (starts_with(line, "POT-Creation-Date:")) {
            continue;
        }
        if (starts_with(line, "Plural-Forms:")) {
            auto pos = line.find("nplurals=");
            if (pos != std::string::npos) {
                nPlurals = line[pos + strlen("nplurals=")] - '0';
            }
        }
        linesKept.emplace_back(line);
    }
    return join(linesKept, '\n');
}

std::map<std::string, std::string> Parser::parsePOFile(FileReader &file) {
    std::map<std::string, std::string> consolidatedEntries;
    std::string *currentStr = nullptr;
    for (auto &&line : LineView(file.view())) {
        if (line.empty()) {
            if (isTranslated()) {
                if (msgid.empty() && msgid_plural.empty()) {
                    auto filteredMetaData = parseMetaData(unescape(msgstr));
                    msgstr = filteredMetaData;
                }
                auto origRepr = originalStringRepresentation();
                auto translatedRepr = translatedStringRepresentation();
                consolidatedEntries[origRepr] = translatedRepr;
            }
            clearCurrentEntry();
        } else if (starts_with(line, "#")) {
            continue;
        } else if (starts_with(line, "msgctxt ")) {
            currentStr = &msgctxt;
            currentStr->append(extractStringContent(line, std::strlen("msgctxt ")));
        } else if (starts_with(line, "msgid ")) {
            currentStr = &msgid;
            currentStr->append(extractStringContent(line, std::strlen("msgid ")));
        } else if (starts_with(line, "msgid_plural ")) {
            currentStr = &msgid_plural;
            currentStr->append(extractStringContent(line, std::strlen("msgid_plural ")));
        } else if (starts_with(line, "msgstr ")) {
            currentStr = &msgstr;
            currentStr->append(extractStringContent(line, std::strlen("msgstr ")));
        } else if (starts_with(line, "msgstr[0] ")) {
            currentStr = &msgstr_plural[0];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[0] ")));
        } else if (starts_with(line, "msgstr[1] ")) {
            currentStr = &msgstr_plural[1];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[1] ")));
        } else if (starts_with(line, "msgstr[2] ")) {
            currentStr = &msgstr_plural[2];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[2] ")));
        } else if (starts_with(line, "msgstr[3] ")) {
            currentStr = &msgstr_plural[3];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[3] ")));
        } else if (starts_with(line, "msgstr[4] ")) {
            currentStr = &msgstr_plural[4];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[4] ")));
        } else if (starts_with(line, "msgstr[5] ")) {
            currentStr = &msgstr_plural[5];
            currentStr->append(extractStringContent(line, std::strlen("msgstr[5] ")));
        } else if (starts_with(line, "\"")) {
            currentStr->append(extractStringContent(line, 0));
        } else {
            throw std::runtime_error(fmt::format("Don't know how to deal with \"{}\"", line));
        }
    }
    return consolidatedEntries;
}
