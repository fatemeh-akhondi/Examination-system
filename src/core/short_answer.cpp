#include "short_answer.hpp"
#include <iostream>

Short_answer::Short_answer(const std::string& text, const std::string& answer, int positive_mark, int negative_mark)
    : Question(text, answer, positive_mark) {
    this->negative_mark = negative_mark;
}

void Short_answer::print_question() {
    std::cout << text << std::endl;
    std::cout << "Positive mark: " << positive_mark << " / Negative mark: " << negative_mark << std::endl;
}