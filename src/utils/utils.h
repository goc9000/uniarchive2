#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cstdarg>
#include <iterator>
#include <vector>

template<typename T>
void appendByMoving(std::vector<T>& destination, std::vector<T>& source)
{
    destination.insert(destination.end(),
                       std::make_move_iterator(source.begin()),
                       std::make_move_iterator(source.end()));
    source.clear();
}

template<typename T>
void appendByMoving(std::vector<T>& destination, std::vector<T>&& source)
{
    destination.insert(destination.end(),
                       std::make_move_iterator(source.begin()),
                       std::make_move_iterator(source.end()));
}

std::string vsprintfToStdString(const char * formatStr, va_list args);

#endif // UTILS_H
