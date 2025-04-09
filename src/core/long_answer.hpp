#ifndef LONG_ANSWER_HPP
#define LONG_ANSWER_HPP

#include "question.hpp"

class Long_answer : public Question {
public:
    Long_answer(const std::string& text, const std::string& answer, int positive_mark);

    void print_question();
    json to_json();

private:
    bool is_graded = false;
};

#endif // LONG_ANSWER_HPP