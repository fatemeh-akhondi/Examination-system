#ifndef SHORT_ANSWER_HPP
#define SHORT_ANSWER_HPP

#include "question.hpp"
#include "../../Lib/json.hpp"
#include <iostream>

class Short_answer : public Question {
public:
    Short_answer(const std::string& text, const std::string& answer, int positive_mark, int negative_mark);
    Short_answer(int id, const std::string& text, const std::string& answer, int positive_mark, int negative_mark);


    void print_question(std::ostream &os);
    static void from_json(json &j);
};

#endif // SHORT_ANSWER_HPP