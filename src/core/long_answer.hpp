#ifndef LONG_ANSWER_HPP
#define LONG_ANSWER_HPP

#include "question.hpp"
#include "../../Lib/json.hpp"

class Long_answer : public Question {
public:
    Long_answer(const std::string& text, const std::string& answer, int positive_mark);
    Long_answer(int id, const std::string& text, const std::string& answer, int positive_mark);

    void print_question();
    json to_json();
    static void from_json(json &j);

private:
    bool is_graded = false;
};

#endif // LONG_ANSWER_HPP