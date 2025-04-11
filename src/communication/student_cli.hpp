#ifndef STUDENT_CLI_HPP
#define STUDENT_CLI_HPP

#include "../core/student.hpp"
#include "exam_cli.hpp"
#include "../utils/tools.hpp"

class Student_CLI {
public:
    Student_CLI(Student* current_student);

    void show_profile_page();

private:
    Student* current_student;

    void execute_command(const std::string command);
    void add_exam_page();
    void handle_view();
    void participate_page();
    void get_results();
};

#endif // STUDENT_CLI_HPP