#ifndef NOT_FOUND_EXCEPTION_HPP
#define NOT_FOUND_EXCEPTION_HPP

#include <string>

#include "exception.hpp"

class Not_found_exception : Exception {
public:
    Not_found_exception() : Exception("Not Found"){};
    Not_found_exception(std::string message) : Exception("Not Found: " + message){};
};

#endif