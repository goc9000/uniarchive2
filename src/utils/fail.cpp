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

void printException(const std::exception& exception)
{
    printLogMessage(LOG_MSG_ERROR, std::string(exception.what()));
}

void printLogMessage(LogMessageType messageType, const std::string& message)
{
    // TODO: temporarily hardcoded color support here. Should use ncurses(?)
    // for more reliability.
    bool useColors = isatty(2);

    std::string prefix = "";
    std::string color = "";
    switch (messageType) {//color = "31;1";
    case LOG_MSG_ERROR:   prefix = "ERROR: ";   color = "31;1"; break;
    case LOG_MSG_WARNING: prefix = "WARNING: "; color = "33;1"; break;
    case LOG_MSG_NOTICE:  prefix = "NOTE: ";    color = "36;1"; break;
    case LOG_MSG_DEBUG:   prefix = "DEBUG: ";   color = "37"; break;
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
