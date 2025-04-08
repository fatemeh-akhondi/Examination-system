#ifndef DATA_EXPORTER_HPP
#define DATA_EXPORTER_HPP

#include <string>

class Data_exporter {
private:
    template <class T>
    void save_data(const std::string& file_path);

    void save_professor() { save_data<Professor>("professor.json"); }
    void save_student() { save_data<Student>("student.json"); };
    void save_exam();
    void save_question();
    void save_exam_response();
};

#endif