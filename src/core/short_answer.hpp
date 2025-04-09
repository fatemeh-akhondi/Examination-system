#ifndef SHORT_ANSWER_HPP
#define SHORT_ANSWER_HPP

#include "question.hpp"

class Short_answer : public Question {
public:
    Short_answer(const std::string& text, const std::string& answer, int positive_mark, int negative_mark);

    void print_question();
};

#endif // SHORT_ANSWER_HPP