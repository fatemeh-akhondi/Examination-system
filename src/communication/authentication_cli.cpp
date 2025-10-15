#include "authentication_cli.hpp"
#include "../exceptions/exception.hpp"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

void Authentication_CLI::show_main_page() {
    cout << "Welcome! you can enter 'logout' at anytime for returning to this page." << endl;
    cout << "Enter 'exit' in this page for finishing the program." << endl;
    string command;

    while (true) {
        cout << "What do you want to do? ('login'/'signup')" << endl;
        getline(cin, command);

        if (command == "exit") {
            break;
        }

        try {
            execute_command(command);
        }
        catch (Exception &e) {
            cout << e.get_message() << endl;
        }
    }
}

void Authentication_CLI::execute_command(string command) {
    if (command == "login") {
        handle_login();
    }
    else if (command == "signup") {
        handle_signup();
    }
    else {
        cout << "Invalid command" << endl;
    }
}

void Authentication_CLI::handle_login() {
    cout << "in which role do you want to login? ('student'/'professor')" << endl;
    string role;
    getline(cin, role);
    string password, id;
    cout << "Enter your username" << endl;
    getline(cin, id);
    cout << "Enter your password" << endl;
    getline(cin, password);

    validate_login_data(role, id, password);

    if (role == "student") {
        Student_CLI new_CLI(Student::get_student(id));
        new_CLI.show_profile_page();
    }
    else if (role == "professor") {
        Professor_CLI new_CLI(Professor::get_professor(id));
        new_CLI.show_profile_page();
    }
}

void Authentication_CLI::validate_login_data(string role, string id, string password) {
    if (role != "professor" && role != "student") {
        throw Exception("invalid role");
    }
    if ((role == "professor" && Professor::get_professor(id) == nullptr) || 
            (role == "student" && Student::get_student(id) == nullptr)) {
        throw Exception("such id does not exist");
    }
    if ((role == "professor" && Professor::get_professor(id)->get_password() != password) || 
            (role == "student" && Student::get_student(id)->get_password() != password)) {
        throw Exception("id and password don't match");
    }

    cout << "login successful!" << endl;
}

void Authentication_CLI::handle_signup() {
    cout << "Enter your role ('professor'/'student'):" << endl;
    string role;
    getline(cin, role);

    if (role == "professor") {
        professor_signup();
    } else if (role == "student") {
        student_signup();
    } else {
        cout << "invalid role" << endl;
    }
}

void Authentication_CLI::professor_signup() {
    string name;
    cout << "enter your name:" << endl;
    getline(cin, name);

    cout << "enter your courses:" << endl;
    string courses;
    getline(cin, courses);
    vector <string> course_list;
    Tools::fill_vector_from_line(course_list, courses);


    string id;
    cout << "enter your username(id):" << endl;
    getline(cin, id);
    while (Professor::get_professor(id) != nullptr) {
        cout << "this id already exists! enter a new one." << endl;
        getline(cin, id);
    }

    cout << "enter your password:" << endl;
    string password;
    getline(cin, password);

    new Professor(name, id, course_list, password);
    
    cout << "signup successful! you will be returned to main page now." << endl;
}

void Authentication_CLI::student_signup() {
    string name;
    cout << "enter your name:" << endl;
    getline(cin, name);

    cout << "enter your field:" << endl;
    string field;
    getline(cin, field);

    string id;
    cout << "enter your username(id):" << endl;
    getline(cin, id);
    while (Student::get_student(id) != nullptr) {
        cout << "this id already exists! enter a new one." << endl;
        getline(cin, id);
    }

    cout << "enter your password:" << endl;
    string password;
    getline(cin, password);

    new Student(name, id, field, password);
    
    cout << "signup successful! you will be returned to main page now." << endl;
}