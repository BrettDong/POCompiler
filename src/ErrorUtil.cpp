#include "ErrorUtil.hpp"

#if defined(_WIN32)
#if !defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include <cerrno>
#include <cstring>
#endif

std::string getSystemError() {
#if defined(_WIN32)
    DWORD ec = GetLastError();
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, ec, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, nullptr);
    std::string errMsg(static_cast<char *>(lpMsgBuf));
    LocalFree(lpMsgBuf);
    return errMsg;
#else
    return std::string(std::strerror(errno));
#endif
}
