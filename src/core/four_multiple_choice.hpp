#ifndef FOUR_MULTIPLE_CHOICE_HPP
#define FOUR_MULTIPLE_CHOICE_HPP

#include "question.hpp"
#include <string>

class Four_multiple_choice: public Question {
public:
    Four_multiple_choice(string text, string answer, int positive_mark,
            int negative_mark, string opstions[4]);

    void shuffle_options();
    void print_question();
    json to_json();
private:
    string options[4];
};
    

#endif // FOUR_MULTIPLE_CHOICE_HPP