#include "tools.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace Tools {
    
int get_integer_input() {
    int value;
    string input;
    while (true) {
        getline(cin, input);
        std::stringstream ss(input);

        if (ss >> value && ss.eof()) {
            return value;
        }
        cout << "invalid input. please enter an integer." << endl;
    }
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
    std::stringstream ss(line);
    string iterator_string;

    while (getline(ss, iterator_string, ' ')) {
        list.push_back(iterator_string);
    }
}

};
