#include <iostream>
#include <unistd.h>

#include "fail.h"

#include "utils/utils.h"


void fail(const char* formatStr, ...)
{
    va_list vl;

    va_start(vl, formatStr);
    std::string message = vsprintfToStdString(formatStr, vl);
    va_end(vl);

    throw std::runtime_error(message);
}

void warn(const char* formatStr, ...)
{
    va_list vl;

    va_start(vl, formatStr);
    std::string message = vsprintfToStdString(formatStr, vl);
    va_end(vl);

    printLogMessage(LogMessageType::WARNING, message);
}

void note(const char* formatStr, ...)
{
    va_list vl;

    va_start(vl, formatStr);
    std::string message = vsprintfToStdString(formatStr, vl);
    va_end(vl);

    printLogMessage(LogMessageType::NOTICE, message);
}

void debug(const char* formatStr, ...)
{
    va_list vl;

    va_start(vl, formatStr);
    std::string message = vsprintfToStdString(formatStr, vl);
    va_end(vl);

    printLogMessage(LogMessageType::DEBUG, message);
}

void printException(const std::exception& exception)
{
    printLogMessage(LogMessageType::ERROR, std::string(exception.what()));
}

void printLogMessage(LogMessageType messageType, const std::string& message)
{
    // TODO: temporarily hardcoded color support here. Should use ncurses(?)
    // for more reliability.
    bool useColors = isatty(2);

    std::string prefix = "";
    std::string color = "";
    switch (messageType) {
    case LogMessageType::ERROR:   prefix = "ERROR: ";   color = "31;1"; break;
    case LogMessageType::WARNING: prefix = "WARNING: "; color = "33;1"; break;
    case LogMessageType::NOTICE:  prefix = "NOTE: ";    color = "36;1"; break;
    case LogMessageType::DEBUG:   prefix = "DEBUG: ";   color = "37"; break;
    }

    if (useColors) {
        std::cerr << "\033[" << color << "m";
    }

    std::cerr << prefix << message;

    if (useColors) {
        std::cerr << "\033[0m";
    }

    std::cerr << std::endl;
}
