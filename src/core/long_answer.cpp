#include "long_answer.hpp"
#include <iostream>

Long_answer::Long_answer(const std::string& text, const std::string& answer, int positive_mark)
    : Question(text, answer, positive_mark) {}

void Long_answer::print_question() {
    std::cout << text << std::endl;
    std::cout << "Positive mark: " << positive_mark << std::endl;
}

json Long_answer::to_json() {
    json j = Question::to_json();
    j["is_graded"] = is_graded;
    return j;
}