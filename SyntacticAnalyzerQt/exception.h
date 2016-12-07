#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    Exception(std::string exc);

    std::string exeption_string;
};

#endif // EXCEPTION_H
