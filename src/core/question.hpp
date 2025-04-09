#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>
#include "../../Lib/json.hpp"
#include "instance_manager.hpp"

using json = nlohmann::json;

class Question : public Instance_manager<Question> {
public:
    Question(const std::string& text, const std::string& answer, int positive_mark);

    int get_positive_mark() { return positive_mark; }
    int get_negative_mark() { return negative_mark; }
    std::string get_text() { return text; }
    std::string get_answer() { return answer; }
    int get_id() { return id; }

    virtual void print_question() = 0;
    virtual json to_json();

protected:
    std::string text;
    std::string answer;
    int id;
    int positive_mark;
    int negative_mark = 0;

private:
    static int instance_count;
};

#endif