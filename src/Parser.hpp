#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <array>
#include <map>
#include <string>

class FileReader;

class Parser {
  protected:
    static std::string_view extractStringContent(std::string_view line, std::size_t prefix_length);

    static constexpr std::size_t maxPluralForms = 6;
    std::string msgctxt;
    std::string msgid;
    std::string msgid_plural;
    std::string msgstr;
    std::array<std::string, maxPluralForms> msgstr_plural;
    std::size_t nPlurals = 1;

    void clearCurrentEntry();
    bool isSingular() const;
    bool isPlural() const;
    bool isTranslated() const;
    std::string originalStringRepresentation() const;
    std::string translatedStringRepresentation() const;
    std::string parseMetaData(const std::string &metaData);

  public:
    std::map<std::string, std::string> parsePOFile(FileReader &file);
};

#endif
