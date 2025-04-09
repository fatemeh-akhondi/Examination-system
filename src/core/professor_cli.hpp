#ifndef PROFESSOR_CLI_HPP
#define PROFESSOR_CLI_HPP

#include "professor.hpp"
#include "exam.hpp"
#include "question_creator.hpp"
#include "../utils/tools.hpp"

class Professor_CLI {
public:
    Professor_CLI(Professor* current_professor);

    void show_profile_page();

private:
    Professor* current_professor;

    void show_exam_building_page();
    Exam* build_exam();
    void show_question_modification_page(Exam* current_exam);
    void show_add_question_page(Exam* current_exam);
    void show_remove_question_page(Exam* current_exam);
    int show_exam_view_page();
    void print_exam(int id);
    void edit_exam();
};

#endif // PROFESSOR_CLI_HPP