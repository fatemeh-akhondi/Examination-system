#include "exception.hpp"

Exception::Exception() { message = ""; }

Exception::Exception(std::string message): message(message) {}

std::string Exception::get_message() { return message; }
