#include "data_exporter.hpp"
#include <iostream>

template <class T>
void Data_exporter::save_data(const std::string& file_path) {
    //checking if the file is empty to create empty json in it.
    bool is_empty = 0;
    std::ifstream check_file(file_path);
    if (check_file.peek() == std::ifstream::traits_type::eof()) { 
        is_empty = 1;
    }
    check_file.close();

    json data_base;
    

    // cout << "before exporting file" << endl;
    if (is_empty) {
        data_base = json::array();
    }
    // else {
    //     std::ifstream read_file(file_path); // Open file in read mode
    //     read_file >> data_base;            // Read the JSON data
    //     read_file.close();
    //     if (read_file.is_open()) {
    //         cout << "file is open " << file_path << endl;
    //     }
    // }

    //update data_basse
    ofstream file(file_path);
    for (auto i : Instance_manager<T>::get_instance_list()) {
        data_base.push_back(i->to_json());
    }

    file << data_base.dump(4) << std::endl;

    file.close();
}

// Explicit template instantiations
template void Data_exporter::save_data<Professor>(const std::string& file_path);
template void Data_exporter::save_data<Student>(const std::string& file_path);
template void Data_exporter::save_data<Exam>(const std::string& file_path);
template void Data_exporter::save_data<Question>(const std::string& file_path);
template void Data_exporter::save_data<Exam_response>(const std::string& file_path);