#include "exam_response.hpp"
#include "long_answer.hpp"
#include <typeinfo>
#include <fstream>
#include <math.h>

int Exam_response::instance_count = 0;
std::unordered_map<int, Exam_response*> Exam_response::id_to_pointer;

Exam_response::Exam_response(string submitter_id, int exam_id, vector <Question*> questions):
     submitter_id(submitter_id), exam_id(exam_id) {
    instance_count = max(instance_count, id);
    id = ++instance_count;
    for (auto question : questions) {
        Question_response question_response = {question, "", question->get_answer(), 0};
        question_responses.push_back(question_response);
    }
    id_to_pointer[id] = this;
}

Exam_response::Exam_response(int id, string submitter_id, int exam_id, float score,
     vector <Question_response> question_responses) {
    this->id = id;
    ++instance_count;
    id_to_pointer[id] = this;

    this->question_responses = question_responses;
    this->submitter_id = submitter_id;
    this->exam_id = exam_id;
    this->score = score;
}

Exam_response* Exam_response::get_exam_response(int id) {
    if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
        return nullptr;
    return id_to_pointer[id];
}

void Exam_response::calculate_score() {
    int total_points = 0, achieved_points = 0;
    for (auto& response : question_responses) {
        total_points += response.question->get_positive_mark();

        if (typeid(*response.question) == typeid(Long_answer)) {
            achieved_points += response.score;
            continue;
        }

        string user_answer = response.answer;
        if (response.correct_answer == user_answer) {
            response.score = response.question->get_positive_mark();
            achieved_points += response.question->get_positive_mark();
        }
        else if (user_answer != "") {
            response.score = -abs(response.question->get_negative_mark());
            achieved_points -= abs(response.question->get_negative_mark());
        }
    }


    score = (float) 20 * achieved_points / total_points;
}

void Exam_response::build_response_text(string file_path) {
    calculate_score();

    ofstream file(file_path);
    for (auto response : question_responses) {
        file << "Statement: ";
        response.question->print_question(file);
        file << "Your answer: " << response.answer << endl;
        file << "Correct answer: " << response.question->get_answer() << endl;
        file << "Score from this question: " << response.score << endl;
        file << endl;
    }

    file << "Total score: " << score << endl;
}

json Exam_response::to_json() {
    json j;
    j["id"] = id;
    j["score"] = score;
    j["submitter_id"] = submitter_id;
    j["exam_id"] = exam_id;

    j["question_responses"] = json::array();
    for (auto response : question_responses) {
        j["question_responses"].push_back({response.question->get_id(), response.answer, response.correct_answer, response.score});
    }
    return j;
}

void Exam_response::from_json(json &j) {
    int id = j["id"].get<int>();
    int exam_id = j["exam_id"].get<int>();
    string submitter_id = j["submitter_id"];
    float score = j["score"].get<float>();

    vector <Question_response> question_responses;
    for (auto item : j["question_responses"]) {
        Question* question = Question::get_question(item[0].get<int>());
        Question_response response = {question, item[1], item[2], item[3].get<int>()};
        question_responses.push_back(response);
    }

    new Exam_response(id, submitter_id, exam_id, score, question_responses);
}