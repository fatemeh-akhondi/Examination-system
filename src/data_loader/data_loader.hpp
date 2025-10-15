#ifndef DATALOADER_HPP
#define DATALOADER_HPP

#include "../../Lib/json.hpp"
#include "../core/professor.hpp"
#include "../core/student.hpp"
#include "../core/exam.hpp"
#include "../core/question.hpp"
#include "../core/exam_response.hpp"

class Data_loader {
public:
    void load_all_data() {
        load_question();
        load_data<Exam_response>("../Data/exam_response.json");
        load_data<Exam>("../Data/exam.json");
        load_data<Professor>("../Data/professor.json");
        load_data<Student>("../Data/student.json");
    };
private:
    template <class T>
    void load_data(const std::string& file_path);
    void load_question();
    nlohmann::json load_data_to_json(const std::string &file_path);
};

#endif