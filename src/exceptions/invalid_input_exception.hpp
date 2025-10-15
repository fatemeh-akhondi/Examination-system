#ifndef INVALID_INPUT_EXCEPTION_HPP
#define INVALID_INPUT_EXCEPTION_HPP

#include <string>

#include "exception.hpp"

class Invalid_input_exception :public Exception {
public:
    Invalid_input_exception() : Exception("Invalid Input"){};
    Invalid_input_exception(std::string message) : Exception("Invalid Input: " + message){};
};

#endif