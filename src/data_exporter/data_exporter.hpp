#ifndef DATA_EXPORTER_HPP
#define DATA_EXPORTER_HPP

#include "../../Lib/json.hpp"
#include "../core/professor.hpp"
#include "../core/student.hpp"
#include "../core/exam.hpp"
#include "../core/question.hpp"
#include "../core/exam_response.hpp"
#include <string>
#include <fstream>

using json = nlohmann::json;

class Data_exporter {
public:
    Data_exporter() = default;

    void save_all_data() {
        save_question();
        save_exam_response();
        save_exam();
        save_professor();
        save_student();
    };
private:
    template <class T>
    void save_data(const std::string& file_path);

    void save_question() { save_data<Question>("../Data/question.json"); };
    void save_exam_response() { save_data<Exam_response>("../Data/exam_response.json"); };
    void save_exam() { save_data<Exam>("../Data/exam.json"); };
    void save_professor() { save_data<Professor>("../Data/professor.json"); };
    void save_student() { save_data<Student>("../Data/student.json"); };
};

#endif