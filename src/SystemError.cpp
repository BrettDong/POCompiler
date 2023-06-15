#include "SystemError.hpp"
#include <cerrno>
#include <cstring>
#include <sstream>
#include <stdexcept>

void throwSystemError(std::string_view funcName, std::string_view srcFileName, int srcLineNumber) {
    std::ostringstream oss;
    oss << "System error in " << funcName << " at " << srcFileName << ":L" << srcLineNumber << ": "
        << std::strerror(errno);
    throw std::runtime_error(oss.str());
}
