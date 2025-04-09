#include "professor.hpp"

unordered_map <string, Professor*> Professor::id_to_pointer;

Professor::Professor(string name, string id, vector <string> course_list, string password): Member(name, id, password), course_list(course_list){
    id_to_pointer[id] = this;
}

void Professor::add_exam(Exam *exam) {
    exams.push_back(exam);
}

Professor* Professor::get_professor(string id) {
    if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
        return nullptr;
    return id_to_pointer[id];
}

bool Professor::has_exam(int id) {
    for (auto exam : exams) {
        if (exam->get_id() == id)
            return true;
    }

    return false;
}

json Professor::to_json() {
    json j = Member::to_json();
    j["course_list"] = course_list;
    j["exams"] = json::array();
    for (auto exam : exams) {
        j["exams"].push_back(exam->get_id());
    }

    return j;
}
