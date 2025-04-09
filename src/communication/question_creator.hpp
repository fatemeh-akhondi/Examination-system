#ifndef QUESTION_CREATOR_HPP
#define QUESTION_CREATOR_HPP

#include "../core/question.hpp"
#include "../core/short_answer.hpp"
#include "../core/long_answer.hpp"
#include "../core/four_multiple_choice.hpp"
#include "../utils/tools.hpp"

class Question_creator {
public:
    Question* create_question();

private:
    void get_question_data(std::string& type, std::string& text, std::string& answer, int& positive_mark, int& negative_mark, std::string (&options)[4]);
    void get_type_specefic_data(string type, int &negative_mark, string (&options)[4]);
};

#endif // QUESTION_CREATOR_HPP