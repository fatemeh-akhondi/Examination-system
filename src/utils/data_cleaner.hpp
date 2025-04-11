#ifndef DATA_CLEANER_HPP
#define DATA_CLEANER_HPP

#include "../core/instance_manager.hpp"
#include "../core/question.hpp"
#include "../core/exam_response.hpp"
#include "../core/exam.hpp"
#include "../core/professor.hpp"
#include "../core/student.hpp"

class Data_cleaner {
public:
    static void delete_all_instances() {
        Instance_manager<Question>::delete_all();
        Instance_manager<Exam_response>::delete_all();
        Instance_manager<Exam>::delete_all();
        Instance_manager<Professor>::delete_all();
        Instance_manager<Student>::delete_all();
    }
};

#endif