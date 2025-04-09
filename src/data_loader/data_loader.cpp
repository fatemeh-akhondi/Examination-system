#include "data_loader.hpp"
#include <fstream>
#include <string>
#include <vector>

using string = std::string;

void Data_loader::load_professor() {
    std::ifstream file("professor.json");
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: professor.json");
    }

    json j;
    file >> j;

    for (const auto& item : j) {
        string name = item["name"];
        string id = item["id"];
        string password = item["password"];
        std::vector<string> course_list = item["course_list"].get<std::vector<string>>();
        new Professor(name, id, course_list, password);
    }

    file.close();
}