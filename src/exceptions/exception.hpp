#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>

class Exception {
public:
    Exception();
    Exception(std::string message);
    std::string get_message();

protected:
    std::string message;
};

#endif