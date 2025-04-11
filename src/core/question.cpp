#include "question.hpp"

int Question::instance_count = 0;
std::unordered_map <int, Question*> Question::id_to_pointer;

Question::Question(const std::string& text, const std::string& answer, int positive_mark, std::string type):
     text(text), answer(answer), positive_mark(positive_mark), type(type) {
    id = ++instance_count;
    id_to_pointer[id] = this;
}


Question::Question(int id, const std::string& text, const std::string& answer, int positive_mark, std::string type):
     text(text), answer(answer), positive_mark(positive_mark), type(type) {
    this->id = id;
    instance_count++;
    id_to_pointer[id] = this;
}

Question::~Question(){}

Question* Question::get_question(int id) {
    if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
        return nullptr;
    return id_to_pointer[id];
}

json Question::to_json() {
    json j;
    j["type"] = type;
    j["text"] = text;
    j["answer"] = answer;
    j["id"] = id;
    j["positive_mark"] = positive_mark;
    j["negative_mark"] = negative_mark;
    return j;
}