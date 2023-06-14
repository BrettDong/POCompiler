#pragma once

#include <string_view>

void throwSystemError(std::string_view funcName, std::string_view srcFileName, int srcLineNumber);

#define THROW_SYSTEM_ERROR() throwSystemError(__PRETTY_FUNCTION__, __FILE__, __LINE__)
