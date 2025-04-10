#include "professor_cli.hpp"
#include "../exceptions/exception.hpp"
#include "../exceptions/invalid_input_exception.hpp"
#include "../exceptions/not_found_exception.hpp"
#include <iostream>
#include <string>

Professor_CLI::Professor_CLI(Professor* current_professor): current_professor(current_professor) {}

void Professor_CLI::show_profile_page() {
    while(true) {
        cout << "enter your request ('create'/'view'/'edit')" << endl;
        string command;
        getline(cin, command);

        try{
            if (command == "logout") return;

            if (command == "create") {
                show_exam_building_page();
            }
            else if (command == "view") {
                if (show_exam_view_page() == -1) //TODO use enum LOGOUT
                    return;
            }
            else if (command == "edit") {
                edit_exam();
            }
            else {
                throw Invalid_input_exception();
            }
        }
        catch(Exception &e) {
            cout << e.get_message() << endl;
        }
    }
}

void Professor_CLI::show_exam_building_page() {
    Exam *new_exam = build_exam();

    cout << "generated unique code for this exam is: " << new_exam->get_unique_code() << endl;

    show_question_modification_page(new_exam);
}

Exam* Professor_CLI::build_exam() {
    cout << "enter exam name:" << endl;
    string name;
    getline(cin, name);
    cout << "enter exam time limit in minutes:" << endl;
    int time_limit;
    time_limit = Tools::get_integer_input();

    Exam *new_exam = new Exam(name, time_limit);
    current_professor->add_exam(new_exam);

    return new_exam;
}

void Professor_CLI::show_question_modification_page(Exam* current_exam) {
    while (true) {
        cout << "You can add or remove questions now. Enter your request ('add'/'remove')." << endl;
        cout << "Enter 'back' to return to the previous page." << endl;
        string command;
        getline(cin, command);

        if (command == "back") {
            return;
        }

        if (command == "add") {
            show_add_question_page(current_exam);
        }
        else if (command == "remove") {
            show_remove_question_page(current_exam);
        }
        else {
            cout << "Invalid Input" << endl;
            continue;
        }
    }
}

void Professor_CLI::show_add_question_page(Exam* current_exam) {
    try {
        Question_creator question_creator;
        Question* question = question_creator.create_question(); 
        current_exam->add_question(question);
        cout << "adding successful! question id is: " << question->get_id() << endl;
    }
    catch (Exception &e) {
        cout << e.get_message() << endl;
    }
}

void Professor_CLI::show_remove_question_page(Exam* current_exam) {
    cout << "Enter question ID to remove:" << endl;
    int id;
    id = Tools::get_integer_input();

    try {
        current_exam->remove_question(id);
        cout << "Question removed successfully!" << endl;
    } catch (Exception& e) {
        cout << e.get_message() << endl;
    }
}

int Professor_CLI::show_exam_view_page() {
    if (current_professor->get_exams().empty()) {
        cout << "you have not added any exam yet!" << endl;
        return 0;
    }
    cout << "exam id's are: ";
    for (auto current_exam : current_professor->get_exams()) {
        cout << current_exam->get_id() << " ";
    }
    cout << endl;
    
    while (true) {
        cout << "enter the exam code you want to see." << endl;
        cout << "enter 'back' for returning to previous page." << endl;
        string command;
        getline(cin, command);

        if (command == "back") {
            return 0;
        }
        else if (command == "logout") { //TODO throw
            return -1;
        }
        else if (!Tools::is_number(command)) {
            cout << "Invalid input" << endl;
            continue;
        }
        
        int requested_exam_code = stoi(command);
        print_exam(requested_exam_code);
    }

    return 0;
}

void Professor_CLI::print_exam(int id) {
    if (!current_professor->has_exam(id)) {
        cout << "no such exam." << endl;
        return;
    }

    Exam* exam = Exam::get_exam(id);

    cout << "Exam name: " << exam->get_name() << endl;
    cout << "Questions: " << endl;

    for (auto question : exam->get_questions()) {
        cout << "statement: ";
        question->print_question();
        cout << "answer: " << question->get_answer() << endl;
        cout << endl;
    }
}

void Professor_CLI::edit_exam() {
    cout << "enter exam code:" << endl;
    int id;
    id = Tools::get_integer_input();

    if (!current_professor->has_exam(id)) {
        throw Not_found_exception();
    }

    Exam* exam = Exam::get_exam(id);

    show_question_modification_page(exam);
}
