#ifndef FAIL_H
#define FAIL_H

#include <stdexcept>

enum LogMessageType {
    LOG_MSG_ERROR,
    LOG_MSG_WARNING,
    LOG_MSG_NOTICE,
    LOG_MSG_DEBUG
};

void fail(const char* formatStr, ...);
void warn(const char* formatStr, ...);
void note(const char* formatStr, ...);
void debug(const char* formatStr, ...);

void printException(const std::exception& exception);
void printLogMessage(LogMessageType messageType, const std::string& message);

#endif // FAIL_H
