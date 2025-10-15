#include "professor_cli.hpp"
#include "../exceptions/exception.hpp"
#include "../exceptions/invalid_input_exception.hpp"
#include "../exceptions/not_found_exception.hpp"
#include <iostream>
#include <string>

Professor_CLI::Professor_CLI(Professor* current_professor): current_professor(current_professor) {}

void Professor_CLI::show_profile_page() {
    while(true) {
        cout << "Enter your request ('create'/'view'/'get-results'/'grade')" << endl;
        string command;
        getline(cin, command);

        try{
            if (command == "logout") return;

            if (command == "create") {
                show_exam_building_page();
            }
            else if (command == "view") {
                if (show_exam_view_page() == -1)
                    return;
            }
            else if (command == "get-results") {
                get_results();
            }
            else if (command == "grade") {
                show_grading_page();
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

    cout << "Generated unique code for this exam is: " << new_exam->get_unique_code() << endl;

    show_question_modification_page(new_exam);
}

Exam* Professor_CLI::build_exam() {
    cout << "Enter exam name:" << endl;
    string name;
    getline(cin, name);
    cout << "Enter exam time limit in minutes:" << endl;
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
        cout << "Adding successful! question id is: " << question->get_id() << endl;
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
        cout << "You have not added any exam yet!" << endl;
        return 0;
    }
    cout << "Exam id's are: ";
    for (auto current_exam : current_professor->get_exams()) {
        cout << current_exam->get_id() << " ";
    }
    cout << endl;
    
    while (true) {
        cout << "Enter the exam code you want to see." << endl;
        cout << "Enter 'back' for returning to previous page." << endl;
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
        cout << "No such exam." << endl;
        return;
    }

    Exam* exam = Exam::get_exam(id);

    cout << "Exam name: " << exam->get_name() << endl;
    cout << "Questions: " << endl;

    for (auto question : exam->get_questions()) {
        cout << "Statement: ";
        question->print_question(cout);
        cout << "Answer: " << question->get_answer() << endl;
        cout << endl;
    }
}

void Professor_CLI::get_results() {
    cout << "Enter exam code:" << endl;
    int id;
    id = Tools::get_integer_input();

    if (!current_professor->has_exam(id)) {
        throw Not_found_exception();
    }
    Exam* exam = Exam::get_exam(id);
    if (exam->get_exam_responses().empty()) {
        throw Exception("No response has been submitted yet!");
    }
    
    string file_path = "../user_files/ranking_professor_" + current_professor->get_id() + "_exam_" + to_string(id) + ".csv";
    exam->get_ranking_csv(file_path);
    cout << "ranking file is saved in: " << file_path << endl;
}

void Professor_CLI::show_grading_page() {
    cout << "Enter exam code:" << endl;
    int id;
    id = Tools::get_integer_input();

    handle_grading_error(id);

    Exam* exam = Exam::get_exam(id);
    cout << "exam name: " << exam->get_name() << endl;
    for (auto exam_response : exam->get_exam_responses()) {
        cout << "submitter id: " << exam_response->get_submitter_id() << endl;
        grade_response(exam_response);
    }

    cout << "Grading finished." << endl;
}

void Professor_CLI::handle_grading_error(int id) {
    if (!current_professor->has_exam(id)) {
        throw Not_found_exception();
    }

    Exam* exam = Exam::get_exam(id);
    if (!exam->has_LA_question()) {
        throw Exception("This exam doesn't have any long-answer questions!");
    }
    if (exam->get_exam_responses().empty()) {
        throw Exception("No response has been submitted yet!");
    }
}


void Professor_CLI::grade_response(Exam_response* exam_response) {
    for (auto& question_response : exam_response->question_responses) {
        Question* question = question_response.question;
        if (typeid(*question) != typeid(Long_answer))
            continue;

        question->print_question(cout);
        cout << "correct answer: " << question_response.correct_answer << endl;
        cout << "user answer: " << question_response.answer << endl;

        cout << "enter score:" << endl;
        int new_score;
        new_score = Tools::get_integer_input();
        question_response.score = new_score;
    }
    cout << "scores for this response have been saved!" << endl;
}