#include "question.hpp"

int Question::instance_count = 0;

json Question::to_json() {
    json j;
    j["text"] = text;
    j["answer"] = answer;
    j["id"] = id;
    j["positive_mark"] = positive_mark;
    j["negative_mark"] = negative_mark;
    return j;
}