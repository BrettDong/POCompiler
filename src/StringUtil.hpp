#pragma once
#ifndef STRING_UTIL_HPP
#define STRING_UTIL_HPP

#include <string>
#include <string_view>
#include <vector>

bool starts_with(std::string_view str, std::string_view prefix);
bool ends_with(std::string_view str, std::string_view suffix);
std::string join(const std::vector<std::string> &lines, char delim);
std::string unescape(std::string_view input);

#endif
