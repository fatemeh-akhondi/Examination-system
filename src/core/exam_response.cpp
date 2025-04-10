#include "exam_response.hpp"
#include "long_answer.hpp"
#include <typeinfo>

int Exam_response::instance_count = 0;
std::unordered_map<int, Exam_response*> Exam_response::id_to_pointer;

Exam_response::Exam_response(string submitter_id, vector <Question*> questions): submitter_id(submitter_id) {
    id = ++instance_count;
    for (auto question : questions) {
        question_responses.push_back({question, ""});
    }
    id_to_pointer[id] = this;
}

Exam_response::Exam_response(int id, string submitter_id, float score,
     vector <Question_response> question_responses):
      submitter_id(submitter_id), question_responses(question_responses) {
    this->id = id;
    ++instance_count;
    id_to_pointer[id] = this;
}

Exam_response* Exam_response::get_exam_response(int id) {
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
        else if (user_answer != "") {
            achieved_points -= response.question->get_negative_mark();
        }
    }

    score = (float) 20 * achieved_points / total_points;
}

json Exam_response::to_json() {
    json j;
    j["id"] = id;
    j["score"] = score;
    j["submitter_id"] = submitter_id;

    j["question_responses"] = json::array();
    for (auto response : question_responses) {
        j["question_responses"].push_back({response.question->get_id(), response.answer});
    }
    return j;
}

void Exam_response::from_json(json &j) {
    int id = j["id"].get<int>();
    string submitter_id = j["submitter_id"];
    float score = j["score"].get<float>();

    vector <Question_response> question_responses;
    for (auto item : j["question_responses"]) {
        Question* question = Question::get_question(item[0].get<int>());
        question_responses.push_back({question, item[1]});
    }

    new Exam_response(id, submitter_id, score, question_responses);
}