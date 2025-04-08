#include "data_exporter.hpp"
#include "../main.cpp"
#include "../json.hpp"
#include <fstream>

using json = nlohmann::json;

template <class T>
void Data_exporter::save_data(const std::string& file_path) {
    std::fstream file(file_path, std::ios::app); // to append data at the end of the file

    if (file.peek() == std::ifstream::traits_type::eof()) { // if the file is empty, create an empty JSON array
        json j = json::array();
        file << j.dump(4) << std::endl;
    }

    for (const auto& i : Trackable_object<T>::instance_list) {
        file << i->to_json().dump(4) << std::endl;
    }

    file.close();
}