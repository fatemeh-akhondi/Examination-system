#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <vector>
#include <string>

namespace Tools {
int get_integer_input();
bool check_time(time_t &start_time, int time_limit);

//fills vector from string by separating string using whitespace
void fill_vector_from_line(std::vector <std::string> &list, std::string line);
};

#endif
