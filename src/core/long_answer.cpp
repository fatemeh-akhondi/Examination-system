#include "long_answer.hpp"
#include "constants.hpp"
#include <iostream>

Long_answer::Long_answer(const std::string& text, const std::string& answer, int positive_mark)
    : Question(text, answer, positive_mark, question_types::LA) {}

Long_answer::Long_answer(int id, const std::string& text, const std::string& answer, int positive_mark)
    : Question(id, text, answer, positive_mark, question_types::LA) {}


void Long_answer::print_question(std::ostream& os) {
    os << text << std::endl;
    os << "Positive mark: " << positive_mark << std::endl;
}

json Long_answer::to_json() {
    json j = Question::to_json();
    j["is_graded"] = is_graded;
    return j;
}

void Long_answer::from_json(json &j) {
    std::string text = j["text"];
    std::string answer = j["answer"];
    int id = j["id"].get<int>();
    int positive_mark = j["positive_mark"].get<int>();
    new Long_answer(id, text, answer, positive_mark);
}