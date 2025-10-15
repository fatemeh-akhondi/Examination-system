#include "question_creator.hpp"
#include "../exceptions/not_found_exception.hpp"
#include <iostream>

using namespace std;

Question* Question_creator::create_question() {
    string type, text, answer, options[4];
    int positive_mark, negative_mark;
    get_question_data(type, text, answer, positive_mark, negative_mark, options);

    Question* new_question;
    if (type == "short-answer" || type == "SA") {
        new_question = new Short_answer(text, answer, positive_mark, negative_mark);
    }
    else if (type == "long-answer" || type == "LA") {
        new_question = new Long_answer(text, answer, positive_mark);
    }
    else if (type == "four-multiple-choice" || type == "FMC") {
        new_question = new Four_multiple_choice(text, answer, positive_mark, negative_mark, options);
    }
    else {
        throw Not_found_exception();
    }
    return new_question; 
}

void Question_creator::get_question_data(string &type, string &text, string &answer, int &positive_mark, int &negative_mark, string (&options)[4]) {
    cout << "Enter question type('short-answer(SA)'/'long-answer(LA)'/'four-multiple-choice(FMC)')" << endl;
    getline(cin, type);

    cout << "Enter question text: " << endl;
    getline(cin, text);
    cout << "Enter question positive mark:" << endl;
    positive_mark = Tools::get_integer_input();

    get_type_specefic_data(type, negative_mark, options);

    cout << "Enter answer:" << endl;
    getline(cin, answer);
}

void Question_creator::get_type_specefic_data(string type, int &negative_mark, string (&options)[4]) { //gets type-specefic data for adding question from user
    if (type == "short-answer" || type == "SA" || type == "four-multiple-choice" || type == "FMC") {
        cout << "Enter negative mark:" << endl;
        negative_mark = Tools::get_integer_input();
    }
    if (type == "four-multiple-choice" || type == "FMC") {
        for (int i = 0; i < 4; i++) {
            cout << "Enter option " << i + 1 << ": " << endl;
            getline(cin, options[i]);
        }
    }
}
