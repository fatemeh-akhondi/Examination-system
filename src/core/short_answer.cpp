#include "short_answer.hpp"
#include "constants.hpp"
#include <iostream>

Short_answer::Short_answer(const std::string& text, const std::string& answer, int positive_mark, int negative_mark)
    : Question(text, answer, positive_mark, question_types::SA) {
    this->negative_mark = negative_mark;
}

Short_answer::Short_answer(int id, const std::string& text, const std::string& answer, int positive_mark, int negative_mark)
    : Question(id, text, answer, positive_mark, question_types::SA) {
    this->negative_mark = negative_mark;
}

void Short_answer::print_question() {
    std::cout << text << std::endl;
    std::cout << "Positive mark: " << positive_mark << " / Negative mark: " << negative_mark << std::endl;
}

void Short_answer::from_json(json &j) {
    std::string text = j["text"];
    std::string answer = j["answer"];
    int id = j["id"].get<int>();
    int positive_mark = j["positive_mark"].get<int>();
    int negative_mark = j["negative_mark"].get<int>();
    new Short_answer(id, text, answer, positive_mark, negative_mark);
}