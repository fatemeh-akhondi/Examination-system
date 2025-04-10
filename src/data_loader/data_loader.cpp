#include "data_loader.hpp"
#include "../core/constants.hpp"
#include "../core/short_answer.hpp"
#include "../core/long_answer.hpp"
#include "../core/four_multiple_choice.hpp"
#include <fstream>
#include <string>
#include <vector>

using namespace std;
using json = nlohmann::json;

void Data_loader::load_question() {
    string file_path = "../Data/question.json";
    json json_data = load_data_to_json(file_path);

    if (json_data.empty()) {
        return;
    }

    for (json item : json_data) {
        string type = item["type"];

        if (type == question_types::SA) {
            Short_answer::from_json(item);
        }
        if (type == question_types::LA) {
            Long_answer::from_json(item);
        }
        if (type == question_types::FMC) {
            Four_multiple_choice::from_json(item);
        }  
    }
}

template <class T>
void Data_loader::load_data(const std::string &file_path) {
    json json_data = load_data_to_json(file_path);

    if (json_data.empty()) {
        return;
    }

    for (json item : json_data) {
        T::from_json(item);
    }

}

json Data_loader::load_data_to_json(const std::string &file_path) {
    //check if the file is empty
    bool is_empty = 0;
    std::ifstream check_file(file_path);
    if (check_file.peek() == std::ifstream::traits_type::eof()) {
        is_empty = 1;
    }
    check_file.close();
    
    //import file data into json object
    ifstream file(file_path);
    json json_data;
    if (is_empty) {
        json_data = json::array();
    }
    else {
        file >> json_data;
    }
    file.close();


    return json_data;
}

// Explicit template instantiations
template void Data_loader::load_data<Professor>(const std::string& file_path);
template void Data_loader::load_data<Student>(const std::string& file_path);
template void Data_loader::load_data<Exam>(const std::string& file_path);
template void Data_loader::load_data<Exam_response>(const std::string& file_path);