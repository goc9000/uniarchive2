#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdarg>
#include <iterator>
#include <vector>

template<typename T>
void appendByMoving(std::vector<T>& destination, std::vector<T>& source)
{
    std::move(source.begin(), source.end(), std::back_inserter(destination));
    source.clear();
}

template<typename T>
void appendByMoving(std::vector<T>& destination, std::vector<T>&& source)
{
    std::move(source.begin(), source.end(), std::back_inserter(destination));
}

std::string vsprintfToStdString(const char* formatStr, va_list args);

#endif // UTILS_H
