#include "exam.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <typeinfo>
#include "../exceptions/not_found_exception.hpp"
#include "long_answer.hpp"
#include "short_answer.hpp"
#include "four_multiple_choice.hpp"
#include "constants.hpp"

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

std::unordered_map<int, Exam*> Exam::id_to_exam;

Exam::Exam(string name, int time_limit): name(name), time_limit(time_limit) { 
    id = generate_unique_code();

    id_to_exam[id] = this;
}

int Exam::generate_unique_code() {
    int code = rng() % big_number;
    while (!id_to_exam.empty() && id_to_exam.find(code) != id_to_exam.end()) {
        code = rng() % big_number;
    }

    return code;
}

Exam* Exam::get_exam(int id) {
    if (id_to_exam.empty() || id_to_exam.find(id) == id_to_exam.end())
        return nullptr;
    return id_to_exam[id];
}

void Exam::add_question(Question* new_question) {
    questions.push_back(new_question);
}

void Exam::remove_question(int question_id) {
    if (questions.empty())
        throw Not_found_exception("questions list is empty!");

    int index = -1;
    for (int i = 0; i < questions.size(); i++) {
        if (questions[i]->get_id() == question_id)
            index = i;
    }
    if (index == -1)
        throw Not_found_exception();
    
    questions.erase(questions.begin() + index);
    // delete question;
    //TODO is it safe
}

void Exam::print_exam() {
    cout << this->name << ":" << endl;

    for (auto question : this->questions) {
        question->print_question();
        cout << question->get_answer() << endl;
    }
}

void Exam::prepare_questions() { //reorders questions to be sorted by type, also shuffles Four multiple choice question options
    vector <Question*> FMC;
    vector <Question*> LA;
    vector <Question*> SA;

    for (auto question : questions) {
        if (typeid(*question) == typeid(Short_answer)) {
            SA.push_back(question);
        }
        else if (typeid(*question) == typeid(Long_answer)) {
            LA.push_back(question);
        }
        else if (typeid(*question) == typeid(Four_multiple_choice)) {
            Four_multiple_choice* casted_question = static_cast<Four_multiple_choice*>(question);
            casted_question->shuffle_options();
            FMC.push_back(question);
        }
    }

    questions.clear();
    questions.insert(questions.end(), FMC.begin(), FMC.end());
    questions.insert(questions.end(), SA.begin(), SA.end());
    questions.insert(questions.end(), LA.begin(), LA.end());
}

json Exam::to_json() {
    json j;
    j["name"] = name;
    j["id"] = id;
    j["time-limit"] = time_limit;
    j["Questions"] = json::array();
    for (auto q : questions) {
        j["Questions"].push_back(q->to_json());
    }

    return j;
}