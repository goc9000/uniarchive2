#ifndef FAIL_H
#define FAIL_H

#include <stdexcept>

enum class LogMessageType {
    ERROR,
    WARNING,
    NOTICE,
    DEBUG
};


void fail(const char* formatStr, ...);
void warn(const char* formatStr, ...);
void note(const char* formatStr, ...);
void debug(const char* formatStr, ...);

void printException(const std::exception& exception);
void printLogMessage(LogMessageType messageType, const std::string& message);


class ErrorContext
{
public:
    ErrorContext(const char* formatStr, ...);
    ~ErrorContext();

    void update(const char* formatStr, ...);
    void finish();

protected:
    bool active_;
};

#endif // FAIL_H
