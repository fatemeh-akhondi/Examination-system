#include "exam_response.hpp"
#include "long_answer.hpp"
#include <typeinfo>

int Exam_response::instance_count = 0;
std::unordered_map<std::string, Exam_response*> Exam_response::id_to_pointer;

Exam_response::Exam_response(string submitter_id, vector <Question*> questions): submitter_id(submitter_id) {
    id = ++instance_count;
    for (auto question : questions) {
        question_responses.push_back({question, ""});
    }
}

Exam_response* Exam_response::get_student(string id) {
    if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
    return nullptr;
    return id_to_pointer[id];
}

void Exam_response::calculate_score() {
    int total_points = 0, achieved_points = 0;
    for (auto response : question_responses) {
        total_points += response.question->get_positive_mark();

        if (typeid(*response.question) == typeid(Long_answer))
            continue;

        string correct_answer = response.question->get_answer();
        string user_answer = response.answer;
        if (correct_answer == user_answer) {
            achieved_points += response.question->get_positive_mark();
        }
        else {
            achieved_points -= response.question->get_negative_mark();
        }
    }

    score = (float) 20 * achieved_points / total_points;
}

json Exam_response::to_json() {
    json j;
    j["id"] = id;
    j["score"] = score;

    return j;
}