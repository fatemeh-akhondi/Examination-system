#include "student.hpp"
#include "exceptions/not_found_exception.hpp"

using namespace std;

std::unordered_map<std::string, Student*> Student::id_to_pointer;

Student::Student(string name, string id, string study_field, string password): Member(name, id, password), study_field(study_field){
    id_to_pointer[id] = this;
}
void Student::add_exam_response(Exam_response* response) {
    exam_responses.push_back(response);
}

void Student::add_exam(int id) {
    if (Exam::get_exam(id) == nullptr) {
        throw Not_found_exception("no such exam.");
    }
    exams.push_back(Exam::get_exam(id));
}

Student* Student::get_student(string id) {
    if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
    return nullptr;
    return id_to_pointer[id];
}

json Student::to_json() {
    json j = Member::to_json();
    j["study_field"] = study_field;
    j["exams"] = json::array();
    for (auto exam : exams) {
        j["exams"].push_back(exam->get_id());
    }
    j["exam_responses"] = json::array();
    for (auto response : exam_responses) {
        j["exam_responses"].push_back(response->get_id());
    }

    return j;
}
