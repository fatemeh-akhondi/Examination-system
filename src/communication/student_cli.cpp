#include "student_cli.hpp"
#include "../exceptions/exception.hpp"
#include "../exceptions/not_found_exception.hpp"
#include "../exceptions/invalid_input_exception.hpp"
#include <iostream>
#include <string>

Student_CLI::Student_CLI(Student* current_student) : current_student(current_student) {}

void Student_CLI::show_profile_page() {
    while (true) {
        cout << "enter your request('add'/'view'/'participate'/'get-results')" << endl;
        string command;
        getline(cin, command);

        if (command == "logout")
            return;

        try {
            execute_command(command);
        }
        catch (Exception &e) {
            cout << e.get_message() << endl;
        }
    }
}

void Student_CLI::execute_command(string command) {
    if (command == "add") {
        add_exam_page();
    }
    else if (command == "view") {
        handle_view();
    }
    else if (command == "participate") {
        participate_page();
    } 
    else if (command == "get-results") {
        get_results();
    }
    else {
        throw Invalid_input_exception();
    }
}

void Student_CLI::add_exam_page() {
    cout << "enter exam code:" << endl;
    int id;
    id = Tools::get_integer_input();

    if (current_student->has_added_exam(id)) {
        throw Exception("you have already added this exam.");
    }

    current_student->add_exam(id);
    cout << "added successfully!" << endl;
}

void Student_CLI::handle_view() {
    if (current_student->get_exams().empty()) {
        cout << "you have not added any exam yet!" << endl;
        return;
    }
    cout << "exam id's are: ";
    for (auto exam : current_student->get_exams()) {
        cout << exam->get_id() << " ";
    }
    cout << endl;
}

void Student_CLI::participate_page() {
    cout << "enter exam code:" << endl;
    int id;
    id = Tools::get_integer_input();

    Exam* current_exam = Exam::get_exam(id);
    if (current_exam == nullptr || !current_student->has_added_exam(id)) {
        throw Not_found_exception("no such exam.");
    }
    if (current_student->get_exam_response_by_exam_id(id) != nullptr) {
        throw Exception("you have already participated in this exam.");
    }


    Exam_CLI exam_CLI(current_exam);
    Exam_response* response = exam_CLI.hold_exam(current_student->get_id());
    current_student->add_exam_response(response);
    current_exam->add_question_response(
        response);
}

void Student_CLI::get_results() {
    cout << "enter exam code:" << endl;
    int id;
    id = Tools::get_integer_input();

    Exam_response* response = current_student->get_exam_response_by_exam_id(id);
    if (response == nullptr) {
        throw Not_found_exception("no such exam.");
    }

    Exam* exam = Exam::get_exam(response->get_exam_id());
    
    string ranking_file_path = "../user_files/ranking_student_" + response->get_submitter_id() + "_exam_" + to_string(response->get_exam_id()) + ".csv";
    exam->get_ranking_csv(ranking_file_path);
    cout << "the file of exam ranking has been saved in: " << ranking_file_path << endl;

    string answers_file_path = "../user_files/answers_student_" + response->get_submitter_id() + "_exam_" + to_string(response->get_exam_id()) + ".txt";
    response->build_response_text(answers_file_path);  
    cout << "the file of your answers has been saved in: " << answers_file_path << endl;
}