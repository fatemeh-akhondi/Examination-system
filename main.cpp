#include <iostream>
#include <vector>
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <ctime>
#include <typeinfo>
#include <algorithm>
#include "exceptions/exception.hpp"
#include "exceptions/not_found_exception.hpp"

#define big_number (int) 1e9 + 7

using namespace std;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template <typename T>
class Trackable_object {
public:
    static unordered_set <T*> instance_list;

    Trackable_object() {
        instance_list.insert(static_cast<T*>(this));
    }

    static void delete_all() {
        vector <T*> temporary_list(0);
        for (auto current : instance_list)
            temporary_list.push_back(current);

        for (int list_index = 0; list_index < temporary_list.size(); list_index++) {
            delete temporary_list[list_index];
        }
    }

    ~Trackable_object() {
        instance_list.erase(static_cast<T*>(this));
    }
};

namespace Tools {
    int get_integer_input() {
        int value;
        string input;
        while (true) {
            getline(cin, input);
            stringstream ss(input);

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

    void fill_vector_from_line(vector <string> &list, string line) { //fills vector from string by separating string using whitespace
        stringstream ss(line);
        string iterator_string;

        while (getline(ss, iterator_string, ' ')) {
            list.push_back(iterator_string);
        }
    }
};



class Question: public Trackable_object<Question> {
public:
    string text;
    string answer;
    int id;

    Question(string text, string answer, int positive_mark):
     text(text), answer(answer), positive_mark(positive_mark) {
        this->id = ++instance_count;
    }

    int get_positive_mark() const { return positive_mark; }

    int get_negative_mark() const { return negative_mark; }

    virtual void print_question() = 0;
protected:
    int positive_mark;
    int negative_mark = 0;
private:
    static int instance_count;
};

class Four_multiple_choice: public Question {
public:
    Four_multiple_choice(string text, string answer, int positive_mark,
         int negative_mark, string opstions[4]): Question(text, answer, positive_mark) {
            this->negative_mark = negative_mark;

            for (int i = 0; i < 4; i++) {
                this->options[i] = opstions[i];
            }
    }

    void shuffle_options() {
        int shuffled_order[4] = {1, 2, 3, 4};
        shuffle(shuffled_order, shuffled_order + 4, rng);

        //rerder options according to shuffled order
        string tmp[4];
        for (int i = 0; i < 4; i++) tmp[i] = options[i];
        for (int i = 0; i < 4; i++) options[i] = tmp[shuffled_order[i] - 1];
        
        //modify answer after reorder
        for (int i = 0; i < 4; i++) {
            if (to_string(shuffled_order[i]) == answer) {
                answer = to_string(i + 1);
            }
        }
    }

    void print_question() {
        cout << text << endl;
        for (int i = 0; i < 4; i++) {
            cout << i + 1 << ": " << options[i] << endl;
        }
        cout << "positive mark: " << positive_mark << " / negative mark: " << negative_mark << endl;
    }
private:
    string options[4];
    bool is_graded = 0;
};

class Short_answer: public Question {
public:
    Short_answer(string text, string answer, int positive_mark, int negative_mark):
        Question(text, answer, positive_mark) {
            this->negative_mark = negative_mark;
        }

    void print_question() {
        cout << text << endl;
        cout << "positive mark: " << positive_mark << " / negative mark :" << negative_mark << endl;
    }
private:
};

class Long_answer: public Question {
public:
    Long_answer(string text, string answer, int positive_mark):
     Question(text, answer, positive_mark) {

    }

    void print_question() {
        cout << text << endl;
        cout << "positive mark: " << positive_mark << endl;
    }
private:
};

struct Question_response {
    Question* question;
    string answer;
};
//TODO using questionid = int
class Exam: public Trackable_object<Exam> {
public:
    Exam(string name, int time_limit): name(name), time_limit(time_limit) { 
        id = generate_unique_code();

        id_to_exam[id] = this;
    }

    static Exam* get_exam(int id) {
        if (id_to_exam.empty() || id_to_exam.find(id) == id_to_exam.end())
            return nullptr;
        return id_to_exam[id];
    }

    int get_unique_code() {
        return id;
    }

    vector <Question*> get_questions() { return questions; }

    int get_id() { return id; }

    int get_time_limit() { return time_limit; }

    string get_name() { return name; }
    
    void add_question(Question* new_question) {
        questions.push_back(new_question);
    }

    void remove_question(int id) {
        if (questions.empty())
            throw Not_found_exception("questions list is empty!");

        int index = -1;
        for (int i = 0; i < questions.size(); i++) {
            if (questions[i]->id == id)
                index = i;
        }
        if (id == -1)
            throw Not_found_exception();
        
        questions.erase(questions.begin() + index);
        // delete question;
        //TODO is it safe
    }

    void print_exam() {
        cout << this->name << ":" << endl;

        for (auto question : this->questions) {
            question->print_question();
            cout << question->answer << endl;
        }
    }

    void prepare_questions() { //reorders questions to be sorted by type, also shuffles Four multiple choice question options
        vector <Question*> FMC;
        vector <Question*> LA;
        vector <Question*> SA;

        for (auto question : questions) {
            if (typeid(*question) == typeid(Short_answer)) {
                SA.push_back(question);
            }
            else if (typeid(*question) == typeid(Long_answer)) {
                LA.push_back(question);
            }
            else if (typeid(*question) == typeid(Four_multiple_choice)) {
                Four_multiple_choice* casted_question = static_cast<Four_multiple_choice*>(question);
                casted_question->shuffle_options();
                FMC.push_back(question);
            }
        }

        questions.clear();
        questions.insert(questions.end(), LA.begin(), LA.end());
        questions.insert(questions.end(), SA.begin(), SA.end());
        questions.insert(questions.end(), FMC.begin(), FMC.end());
    }
private:
    static unordered_map <int, Exam*> id_to_exam;
    vector <Question*> questions;
    string name;
    int time_limit;
    int id;

    int generate_unique_code() {
        int code = rng() % big_number;
        while (!id_to_exam.empty() && id_to_exam.find(code) != id_to_exam.end()) {
            code = rng() % big_number;
        }

        return code;
    }
};

class Exam_response : public Trackable_object<Exam_response> {
public:
    vector <Question_response> question_responses;

    Exam_response(Exam* exam) : exam(exam) {
        for (auto question : exam->get_questions()) {
            question_responses.push_back({question, ""});
        }
    }

    float get_score() { return score; }

    float calculate_score() {
        int total_points = 0, achieved_points = 0;
        for (auto response : question_responses) {
            if (typeid(*response.question) == typeid(Long_answer))
                continue;

            string correct_answer = response.question->answer;
            string user_answer = response.answer;
            if (correct_answer == user_answer) {
                achieved_points += response.question->get_positive_mark();
            }
            else {
                achieved_points -= response.question->get_negative_mark();
            }

            total_points += response.question->get_positive_mark();
        }

        return (float) 20 * achieved_points / total_points;
    }

private:
    Exam* exam;
    float score = 0;
};

class Exam_CLI {
public:
    Exam_CLI(Exam* current_exam) {
        this->current_exam = current_exam;
    }

    Exam_response* hold_exam() {
        cout << "press '>' for next question and '<' for previous question and 'end' for finishing the exam" << endl;
        cout << "you have " << current_exam->get_time_limit() << " minutes" << endl;

        time_t start_time;
        time(&start_time);

        //TODO check if time using is correct

        Exam_response *response = process_exam_input(current_exam, start_time);

        cout << "exam finished! you have to wait for long-answer questions to get graded by professor." << endl;
        cout << "your score for this exam is: " << fixed << setprecision(2) << response->calculate_score() << endl;
        return response;
    }

private:
    Exam* current_exam;

    Exam_response* process_exam_input(Exam* current_exam, time_t start_time) {
        current_exam->prepare_questions();

        Exam_response *exam_response = new Exam_response(current_exam);//TODO maybe edit
        vector <Question_response> question_responses = exam_response->question_responses;
        vector <Question*> questions = current_exam->get_questions();
        int index = 0;
        string input;
        while(true) {
            display_response_data(index, questions[index], question_responses[index]);

            getline(cin, input);

            if (exam_ended(start_time, input))
                break;
            if (move_between_question(input, index, question_responses.size() - 1))
                continue;

            question_responses[index].answer = input;

            index++;
            // if (index == question_responses.size()) {
            //     cout << "You have reached the last question. Do you want to submit? ('yes' to submit, 'no' to review)" << endl;
            //     cin >> input;
            //     cin.ignore();
            //     if (input == "yes") break; 

            //     index = 0;
            // }
        }

        return exam_response;
    }

    void display_response_data(int index, Question* question, Question_response& question_response) {
        cout << "question" << index + 1 << ": ";
        question->print_question();

        question_response.question = question; //TODO remove if exam in exam response stays

        if (question_response.answer != "") {
            cout << "your answer: " << question_response.answer << endl;
            cout << "if you want to edit your answer enter it" << endl;
        }
    }

    bool exam_ended(time_t &start_time, string &input) {
        if (!Tools::check_time(start_time, current_exam->get_time_limit())) {
            cout << "your exam time limit has been exceeded! exam finished!" << endl;
            return 1;
        }
        if (input == "end") {
            cout << "are you sure you want to submit the exam? ('yes'/'no')" << endl;
            getline(cin, input);
            if (input == "yes") return 1;
        }

        return 0;
    }

    bool move_between_question(string &input, int &index, int last_index) {
        if (input == "<") {
            index = (index == 0 ? 0 : index - 1);
            return 1;
        }
        if (input == ">") {
            index = (index == last_index ? index : index + 1);
            return 1;
        }

        return 0;
    }

};



class Member {
public:
    Member(string name, string id, string password) {
        this->name = name;
        this->id = id;
        this->password = password;
    }

    string get_password() {
        return password;
    }

protected:
    string name;
    string id;
    string password;
};
       
class Professor: public Member, public Trackable_object<Professor>{
public:
    Professor(string name, string id, vector <string> course_list, string password): Member(name, id, password), course_list(course_list){
        id_to_pointer[id] = this;
    }

    vector <Exam*> & get_exams() { return exams; }

    static Professor* get_professor(string id) {
        if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
            return nullptr;
        return id_to_pointer[id];
    }

    Exam* add_exam(string name, int time_limit) {
        Exam* new_exam = new Exam(name, time_limit);
        exams.push_back(new_exam);

        return new_exam;
    }

    Exam* get_exam(int id) {
        for (auto exam : exams) {
            if (exam->get_id() == id)
                return exam;
        }

        return nullptr;
    }

private:
    static unordered_map <string, Professor*> id_to_pointer;
    vector <Exam*> exams;
    vector <string> course_list;
};

class Professor_CLI {
public:
    Professor_CLI(Professor* current_professor) {
        this->current_professor = current_professor;
    }

    void show_profile_page() {
        while(true) {
            cout << "enter your request ('create'/'view'/'edit')" << endl; //TODO for convinience add an option of editing exams
            string command;
            getline(cin, command);

            try{
                if (command == "logout") return; //TODO

                if (command == "create") {
                    show_exam_building_page();
                }
                if (command == "view") {
                    if (show_exam_view_page() == -1) //TODO use enum LOGOUT
                        return;
                }
                if (command == "edit") {
                    edit_exam();
                }
            }
            catch(Exception &e) {
                cout << e.get_message() << endl;
            }
        }
    }

private:
    Professor* current_professor;

    void show_exam_building_page() {
        cout << "enter exam name:" << endl;
        string name;
        getline(cin, name);
        cout << "enter exam time limit in minutes:" << endl;
        int time_limit;
        time_limit = Tools::get_integer_input();

        Exam *new_exam = current_professor->add_exam(name, time_limit);

        cout << "generated unique code for this exam is: " << new_exam->get_unique_code() << endl;

        show_question_modification_page(new_exam);
    }

    void show_question_modification_page(Exam* current_exam) {
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
                handle_remove_question(current_exam);
            }
            else {
                cout << "Invalid command. Please try again." << endl;
            }
        }
    }

    void handle_remove_question(Exam* current_exam) {
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

    int show_exam_view_page() {
        if (current_professor->get_exams().empty()) {
            cout << "you have not added any exam yet!" << endl;
            return 0;
        }
        cout << "exam id's are: ";
        for (auto current_exam : current_professor->get_exams())
            cout << current_exam->get_id() << " ";
        cout << endl;
        
        while (true) {
            cout << "enter the exam code you want to see." << endl;
            cout << "enter 'back' for returning to previous page." << endl;
            string command;
            getline(cin, command);

            if (command == "back") {
                return 0;
            }
            if (command == "logout") { //TODO throw
                return -1;
            }
            
            int requested_exam_code = stoi(command);
            print_exam(requested_exam_code);
        }

        return 0;
    }

    void print_exam(int id) {
        Exam* exam = current_professor->get_exam(id);
        if (exam == nullptr) {
            throw Not_found_exception();
        }

        cout << exam->get_name() << ":" << endl;

        for (auto question : exam->get_questions()) {
            cout << "statement: ";
            question->print_question();
            cout << "answer: " << question->answer << endl;
            cout << endl;
        }
    }

    void show_add_question_page(Exam* current_exam) {
        cout << "enter question type('short-answer(SA)'/'long-answer(LA)'/'four-multiple-choice(FMC)')" << endl;
        string type;
        getline(cin, type);

        cout << "enter question text: " << endl;
        string text;
        getline(cin, text);
        cout << "enter question positive mark:" << endl;
        int positive_mark;
        positive_mark = Tools::get_integer_input();

        int negative_mark;
        if (type == "short-answer" || type == "SA" || type == "four-multiple-choice" || type == "FMC") {
            cout << "enter negative mark:" << endl;
            negative_mark = Tools::get_integer_input();
        }
        string options[4];
        if (type == "four-multiple-choice" || type == "FMC") {
            for (int i = 0; i < 4; i++) {
                cout << "enter option " << i + 1 << ": " << endl;
                getline(cin, options[i]);
            }
        }

        cout << "enter answer:" << endl;
        string answer;
        getline(cin, answer); //TODO use question az a complete different data type


        Question* new_question = create_question(type, text, answer, positive_mark, negative_mark, options);     
        current_exam->add_question(new_question);
        cout << "adding successful! question id is: " << new_question->id << endl;
    }

    Question* create_question(string type, string text, string answer, int positive_mark, int negative_mark, string options[4]) {
        Question* new_question;
        if (type == "short-answer" || type == "SA") {
            new_question = new Short_answer(text, answer, positive_mark, negative_mark);
        }
        if (type == "long-answer" || type == "LA") {
            new_question = new Long_answer(text, answer, positive_mark);
        }
        if (type == "four-multiple-choice" || type == "FMC") {
            new_question = new Four_multiple_choice(text, answer, positive_mark, negative_mark, options);
        }  
        return new_question; 
    }

    void edit_exam() {
        cout << "enter exam code:" << endl;
        int id;
        id = Tools::get_integer_input();

        Exam* current_exam = current_professor->get_exam(id);
        if (current_exam == nullptr) {
            throw Not_found_exception();
        }

        show_question_modification_page(current_exam);
    }
private:
    //to avoid combination of cin and getline
    string helper_string;
    stringstream helper_stream;
};  

//TODO using examid int
class Student: public Member, public Trackable_object<Student>{
public:
    Student(string name, string id, string study_field, string password): Member(name, id, password), study_field(study_field){
        id_to_pointer[id] = this;
    }

    vector <Exam*> get_exams() {
        return exams;
    }

    vector <Exam_response*> get_exam_responses() {
        return exam_responses;
    }

    void add_exam_response(Exam_response* response) {
        exam_responses.push_back(response);
    }

    static Student* get_student(string id) {
        if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
        return nullptr;
        return id_to_pointer[id];
    }

    void add_exam(int id) {
        if (Exam::get_exam(id) == nullptr) {
            throw Not_found_exception();
        }
        exams.push_back(Exam::get_exam(id));
    }

private:
    static unordered_map <string, Student*> id_to_pointer;
    vector <Exam*> exams;
    vector <Exam_response*> exam_responses;
    string study_field;

friend class Student_CLI;
};

class Student_CLI {
public:
    Student_CLI(Student* current_student) : current_student(current_student) {
    }

    void show_profile_page() {
        while (true) {
            cout << "enter your request('add'/'view'/'participate')" << endl;
            string command;
            getline(cin, command);
    
            if (command == "logout") //TODO throw
                return;
    
            try {
                execute_command(command);
            }
            catch (Exception &e) {
                cout << e.get_message() << endl;
            }

            //TODO wait
        }
    }

private:
    Student* current_student;

    void execute_command(string command) {
        if (command == "add") {
            add_question_page();
        }
        if (command == "view") {
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
        if (command == "participate") {
            participate_page();
        }
    }

    void add_question_page() {
        cout << "enter exam code:" << endl;
        int id;
        id = Tools::get_integer_input();

        current_student->add_exam(id);
        cout << "added successfully!" << endl;
    }

    void participate_page() {
        cout << "enter exam code:" << endl;
        int id;
        id = Tools::get_integer_input();

        Exam* current_exam = Exam::get_exam(id);
        if (current_exam == nullptr) {
            throw Not_found_exception();
        }

        Exam_CLI new_CLI(current_exam);
        Exam_response* response = new_CLI.hold_exam();
        current_student->add_exam_response(response);
    }
};



class Command_line_interface {
public:
    void show_main_page() {
        cout << "Welcome! you can enter 'logout' at anytime to return to main page." << endl;
        cout << "enter 'exit' in main page for finishing the program." << endl;
        string command;

        while (true) {
            cout << "what do you want to do? ('login'/'signup')" << endl;
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

        cleanup();
    }

private:
    void execute_command(string command) {
        if (command == "login") {
            cout << "in which role do you want to login? ('student'/'professor')" << endl;
            string role;
            getline(cin, role);
            string password, id;
            cout << "enter your id" << endl;
            getline(cin, id);
            cout << "enter your password" << endl;
            getline(cin, password);

            show_login_page(role, id, password); //TODO edit

            if (role == "student") {
                Student_CLI new_CLI(Student::get_student(id));
                new_CLI.show_profile_page();
            }
            else if (role == "professor") {
                Professor_CLI new_CLI(Professor::get_professor(id));
                new_CLI.show_profile_page();
            }
            else {
                cout << "no such role" << endl;
            }
        }
        if (command == "signup") {
            show_signup_page();
        }
    }

    bool show_login_page(string role, string id, string password) {
        if (role != "professor" && role != "student") {
            throw Exception("invalid role");
        }
        if ((role == "professor" && Professor::get_professor(id) == nullptr) || 
                role == "student" && Student::get_student(id) == nullptr) {
            throw Exception("such id does not exist");
        }
        if ((role == "professor" && Professor::get_professor(id)->get_password() != password) || 
                role == "student" && Student::get_student(id)->get_password() != password) {
            throw Exception("id and password don't match");
        }

        cout << "login successful!" << endl;

        return 1;
    }

    void show_signup_page() {
        cout << "Enter your role ('professor'/'student'):" << endl;
        string role;
        getline(cin, role);

        // try {
        //     role = TypeValidator::validate_and_get_type(role);
        // } catch (Exception& e) {
        //     cout << e.get_message() << endl;
        //     return;
        // }

        if (role == "professor") {
            professor_signup();
        } else if (role == "student") {
            student_signup();
        }
    }

    void professor_signup() {
        string name;
        cout << "enter your name:" << endl;
        getline(cin, name);

        cout << "enter your courses:" << endl;
        string courses;
        getline(cin, courses);

        vector <string> course_list;
        Tools::fill_vector_from_line(course_list, courses);


        string id;
        cout << "enter your id:" << endl;
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

    void student_signup() {
        string name;
        cout << "enter your name:" << endl;
        getline(cin, name);

        cout << "enter your field:" << endl;
        string field;
        getline(cin, field);

        string id;
        cout << "enter your id:" << endl;
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

    void cleanup() {
        Trackable_object<Professor>::delete_all();
        Trackable_object<Student>::delete_all();
        Trackable_object<Exam>::delete_all();
        Trackable_object<Question>::delete_all();
    }
};

// class TypeValidator {
// public:
//     static const unordered_map<string, string> type_map;

//     static string validate_and_get_type(const string& input) {
//         for (const auto& [full_type, short_form] : type_map) {
//             if (input == full_type || input == short_form) {
//                 return full_type;
//             }
//         }
//         throw Exception("Invalid type: " + input);
//     }
// };

// const unordered_map<string, string> TypeValidator::type_map = {
//     {"professor", "P"},
//     {"student", "S"},
//     {"short-answer", "SA"},
//     {"long-answer", "LA"},
//     {"four-multiple-choice", "FMC"}
// };



//static members initialization
unordered_map <int, Exam*> Exam::id_to_exam;
int Question::instance_count = 0;
template <typename T>
unordered_set <T*> Trackable_object<T>::instance_list;
unordered_map <string, Professor*> Professor::id_to_pointer;
unordered_map <string, Student*> Student::id_to_pointer;



int main() {
    // freopen("in.txt", "r", stdin);
    Command_line_interface *command_line_interface = new Command_line_interface();
    command_line_interface->show_main_page();
}