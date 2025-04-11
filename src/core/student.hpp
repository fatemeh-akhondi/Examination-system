#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "member.hpp"
#include "exam.hpp"
#include "exam_response.hpp"
#include "../../Lib/json.hpp"
#include "instance_manager.hpp"

using json = nlohmann::json;

class Student : public Member, public Instance_manager<Student> {
public:
    Student(string name, string id, string study_field, string password);
    Student(string name, string id, string study_field, string password, vector<Exam*> exams, vector<Exam_response*> exam_responses);

    vector <Exam*> get_exams() { return exams; }
    vector <Exam_response*> get_exam_responses() { return exam_responses; }

    void add_exam_response(Exam_response* response);
    void add_exam(int id);

    static Student* get_student(const std::string id);
    Exam_response* get_exam_response_by_exam_id(int exam_id);
    bool has_added_exam(int id);
    json to_json();
    static void from_json(json &j);

private:
    static std::unordered_map<std::string, Student*> id_to_pointer;
    std::string study_field;
    std::vector<Exam*> exams;
    std::vector<Exam_response*> exam_responses;

    friend class Student_CLI;
};

#endif // STUDENT_HPP