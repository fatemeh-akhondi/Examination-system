#include "tools.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <random>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

std::mt19937 rng(std::random_device{}());

namespace Tools {
    
int get_integer_input() {
    while (true) {
        string input;
        getline(cin, input);
        
        if (is_number(input)) {
            std::stringstream string_stream(input);
            int value;
            string_stream >> value;
            return value;
        }
        cout << "invalid input. please enter an integer." << endl;
    }
}

bool is_number(string s) {
    std::stringstream stream(s);
    int number;
    if (stream >> number && stream.eof()) {
        return true;
    }
    return false;
}

bool check_time(time_t &start_time, int time_limit) {
    time_t current_time;
    time(&current_time);

    if ((current_time - start_time) / 60 > time_limit)
        return false;
    return true;
}

//fills vector from string by separating string using whitespace
void fill_vector_from_line(vector<string> &list, string line) {
    std::stringstream string_stream(line);
    string iterator_string;

    while (getline(string_stream, iterator_string, ' ')) {
        list.push_back(iterator_string);
    }
}

};
