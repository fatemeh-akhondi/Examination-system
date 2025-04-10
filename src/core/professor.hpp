#ifndef PROFESSOR_HPP
#define PROFESSOR_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "member.hpp"
#include "exam.hpp"
#include "../../Lib/json.hpp"
#include "instance_manager.hpp"

class Professor: public Member, public Instance_manager<Professor>{
public:
    Professor(string name, string id, vector <string> course_list, string password);
    Professor(string name, string id, vector <string> course_list, string password, vector <Exam*> exams);

    vector <Exam*> get_exams() { return exams; }

    void add_exam(Exam *exam);

    static Professor* get_professor(string id);
    bool has_exam(int id);
    json to_json();
    static void from_json(json &j);

private:
    static unordered_map <string, Professor*> id_to_pointer;
    vector <string> course_list;
    vector <Exam*> exams;
};

#endif