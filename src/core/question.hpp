#ifndef QUESTION_HPP
#define QUESTION_HPP

#include <string>
#include <unordered_map>
#include <iostream>
#include "../../Lib/json.hpp"
#include "instance_manager.hpp"

using json = nlohmann::json;

class Question : public Instance_manager<Question> {
public:
    Question(const std::string& text, const std::string& answer, int positive_mark, std::string type);
    Question(int id, const std::string& text, const std::string& answer, int positive_mark, std::string type);

    int get_positive_mark() { return positive_mark; }
    int get_negative_mark() { return negative_mark; }
    std::string get_text() { return text; }
    std::string get_answer() { return answer; }
    int get_id() { return id; }

    static Question* get_question(int id);
    virtual void print_question(std::ostream& os) = 0;
    virtual json to_json();

    virtual ~Question();

protected:
    int id;
    std::string text;
    std::string answer;
    int positive_mark;
    std::string type;
    int negative_mark = 0;

private:
    static int instance_count;
    static std::unordered_map <int, Question*> id_to_pointer;
};

#endif