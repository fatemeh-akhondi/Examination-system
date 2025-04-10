#ifndef NOT_FOUND_EXCEPTION_HPP
#define NOT_FOUND_EXCEPTION_HPP

#include <string>

#include "exception.hpp"

class Not_found_exception :public Exception {
public:
    Not_found_exception() : Exception("Item Not Found"){};
    Not_found_exception(std::string message) : Exception("Item Not Found: " + message){};
};

#endif