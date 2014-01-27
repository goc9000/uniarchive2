#include "utils.h"

#include <string>

#define DEFAULT_STR_BUFFER_SIZE 512

std::string vsprintfToStdString(const char * formatStr, va_list args)
{
    char buf[DEFAULT_STR_BUFFER_SIZE];

    va_list argsCopy;
    va_copy(argsCopy, args);

    int strSize = vsnprintf(buf, DEFAULT_STR_BUFFER_SIZE, formatStr, args);
    if (strSize < DEFAULT_STR_BUFFER_SIZE) {
        va_end(argsCopy);
        return std::string(buf);
    }

    char *largerBuf = new char[strSize + 1];

    vsnprintf(largerBuf, strSize + 1, formatStr, argsCopy);
    va_end(argsCopy);

    std::string result(largerBuf);
    delete[] largerBuf;

    return result;
}
