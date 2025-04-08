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
#include <fstream>
#include "exceptions/exception.hpp"
#include "exceptions/not_found_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "json.hpp"

#define big_number (int) 1e9 + 7

using namespace std;
using json = nlohmann::json;

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

    virtual json to_json() {
        json j;
        j["text"] = text;
        j["answer"] = answer;
        j["id"] = id;
        j["positive_mark"] = positive_mark;
        j["negative_mark"] = negative_mark;

        return j;
    }
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

    json to_json() {
        json j = Question::to_json();
        j["options"] = json::array();
        for (int i = 0; i < 4; i++) {
            j["options"].push_back(options[i]);
        }

        return j;
    }
private:
    string options[4];
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

    json to_json() {
        json j = Question::to_json();
        j["is_graded"] = is_graded;
        return j;
    }
private:
    bool is_graded = 0;
};

class Question_creator {
public:
    Question* create_question() {
        string type, text, answer, options[4];
        int positive_mark, negative_mark;
        get_question_data(type, text, answer, positive_mark, negative_mark, options);

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

private:
    void get_question_data(string &type, string &text, string &answer, int &positive_mark, int &negative_mark, string (&options)[4]) {
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
        string options[4];
        get_type_specefic_data(type, negative_mark, options);

        cout << "enter answer:" << endl;
        string answer;
        getline(cin, answer);
    }

    void get_type_specefic_data(string type, int &negative_mark, string (&options)[4]) { //gets type-specefic data for adding question from user
        if (type == "short-answer" || type == "SA") {
            cout << "enter negative mark:" << endl;
            negative_mark = Tools::get_integer_input();
        }
        if (type == "four-multiple-choice" || type == "FMC") {
            for (int i = 0; i < 4; i++) {
                cout << "enter option " << i + 1 << ": " << endl;
                getline(cin, options[i]);
            }
        }
    }

};

struct Question_response {
    Question* question;
    string answer;
};
//TODO using questionid = int
class Exam_response : public Trackable_object<Exam_response> {
public:
    vector <Question_response> question_responses;

    Exam_response(string submitter_id, vector <Question*> questions): submitter_id(submitter_id) {
        id = ++instance_count;
        for (auto question : questions) {
            question_responses.push_back({question, ""});
        }
    }

    float get_score() { return score; }

    int get_id() { return id; }

    static Exam_response* get_student(string id) {
        if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
        return nullptr;
        return id_to_pointer[id];
    }

    void calculate_score() {
        int total_points = 0, achieved_points = 0;
        for (auto response : question_responses) {
            total_points += response.question->get_positive_mark();

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
        }

        score = (float) 20 * achieved_points / total_points;
    }

    json to_json() {
        json j;
        j["id"] = id;
        j["score"] = score;

        return j;
    }

private:
    static unordered_map <string, Exam_response*> id_to_pointer;
    static int instance_count;
    string submitter_id;
    float score = 0;
    int id;
};

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

    void remove_question(int question_id) {
        if (questions.empty())
            throw Not_found_exception("questions list is empty!");

        int index = -1;
        for (int i = 0; i < questions.size(); i++) {
            if (questions[i]->id == question_id)
                index = i;
        }
        if (index == -1)
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
        questions.insert(questions.end(), FMC.begin(), FMC.end());
        questions.insert(questions.end(), SA.begin(), SA.end());
        questions.insert(questions.end(), LA.begin(), LA.end());
    }

    json to_json() {
        json j;
        j["name"] = name;
        j["id"] = id;
        j["time-limit"] = time_limit;
        j["Questions"] = json::array();
        for (auto q : questions) {
            j["Questions"].push_back(q->to_json());
        }

        return j;
    }
private:
    static unordered_map <int, Exam*> id_to_exam;
    vector <Question*> questions;
    vector <Exam_response> responses;
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

class Exam_CLI {
public:
    Exam_CLI(Exam* current_exam) {
        this->current_exam = current_exam;
    }

    Exam_response* hold_exam(string submitter_id) {
        cout << "press '>' for next question and '<' for previous question and 'end' for finishing the exam" << endl;
        cout << "you have " << current_exam->get_time_limit() << " minutes" << endl;

        time_t start_time;
        time(&start_time);

        //TODO check if time using is correct

        Exam_response *response = process_exam_input(submitter_id, current_exam, start_time);

        cout << "exam finished! you have to wait for long-answer questions to get graded by professor." << endl;
        response->calculate_score();
        cout << "your score for this exam is: " << fixed << setprecision(2) << response->get_score() << endl;
        return response;
    }

private:
    Exam* current_exam;

    Exam_response* process_exam_input(string submitter_id, Exam* current_exam, time_t start_time) {
        current_exam->prepare_questions();

        Exam_response *exam_response = new Exam_response(submitter_id, current_exam->get_questions());//TODO maybe edit
        vector <Question_response>& question_responses = exam_response->question_responses;
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
            if (index == question_responses.size()) {
                cout << "You have reached the last question. Do you want to submit? ('yes' to submit, 'no' to review)" << endl;
                getline(cin, input);
                if (input == "yes") break; 

                index = 0;
            }
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

    string get_id() { return id; }

    json to_json() {
        json j;
        j["name"] = name;
        j["id"] = id;
        j["password"] = password;

        return j;
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

    void add_exam(Exam *exam) {
        exams.push_back(exam);
    }

    static Professor* get_professor(string id) {
        if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
            return nullptr;
        return id_to_pointer[id];
    }

    bool has_exam(int id) {
        for (auto exam : exams) {
            if (exam->get_id() == id)
                return true;
        }

        return false;
    }

    json to_json() {
        json j = Member::to_json();
        j["course_list"] = course_list;
        j["exams"] = json::array();
        for (auto exam : exams) {
            j["exams"].push_back(exam->get_id());
        }

        return j;
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

private:
    Professor* current_professor;

    void show_exam_building_page() {
        Exam *new_exam = build_exam();

        cout << "generated unique code for this exam is: " << new_exam->get_unique_code() << endl;

        show_question_modification_page(new_exam);
    }

    Exam* build_exam() {
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
                show_remove_question_page(current_exam);
            }
            else {
                throw Invalid_input_exception();
            }
        }
    }

    void show_add_question_page(Exam* current_exam) {
        Question_creator question_creator;
        Question* question = question_creator.create_question();  

        current_exam->add_question(question);
        cout << "adding successful! question id is: " << question->id << endl;
    }

    void show_remove_question_page(Exam* current_exam) {
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
        if (!current_professor->has_exam(id)) {
            cout << "no such exam." << endl;
            return;
        }

        Exam* exam = Exam::get_exam(id);

        cout << exam->get_name() << ":" << endl;

        for (auto question : exam->get_questions()) {
            cout << "statement: ";
            question->print_question();
            cout << "answer: " << question->answer << endl;
            cout << endl;
        }
    }

    void edit_exam() {
        cout << "enter exam code:" << endl;
        int id;
        id = Tools::get_integer_input();

        if (!current_professor->has_exam(id)) {
            throw Not_found_exception();
        }

        Exam* exam = Exam::get_exam(id);

        show_question_modification_page(exam);
    }

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

    void add_exam(int id) {
        if (Exam::get_exam(id) == nullptr) {
            throw Not_found_exception("no such exam.");
        }
        exams.push_back(Exam::get_exam(id));
    }

    static Student* get_student(string id) {
        if (id_to_pointer.empty() || id_to_pointer.find(id) == id_to_pointer.end())
        return nullptr;
        return id_to_pointer[id];
    }

    json to_json() {
        json j = Member::to_json();
        j["study_field"] = study_field;
        j["exams"] = json::array();
        for (auto exam : exams) {
            j["exams"].push_back(exam->get_id());
        }
        j["exam_responses"] = json::array();
        for (auto response : exam_responses) {
            j["exam_responses"].push_back(response->get_id());
        }

        return j;
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
private:
    Student* current_student;

    void execute_command(string command) {
        if (command == "add") {
            add_exam_page();
        }
        else if (command == "view") {
            handle_view();
        }
        else if (command == "participate") {
            participate_page();
        } 
        else {
            throw Invalid_input_exception();
        }
    }

    void add_exam_page() {
        cout << "enter exam code:" << endl;
        int id;
        id = Tools::get_integer_input();

        current_student->add_exam(id);
        cout << "added successfully!" << endl;
    }

    void handle_view() {
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

    void participate_page() {
        cout << "enter exam code:" << endl;
        int id;
        id = Tools::get_integer_input();

        Exam* current_exam = Exam::get_exam(id);
        if (current_exam == nullptr) {
            throw Not_found_exception("no such exam.");
        }

        Exam_CLI exam_CLI(current_exam);
        Exam_response* response = exam_CLI.hold_exam(current_student->get_id());
        current_student->add_exam_response(response);
    }
};



class Authentication_CLI {
public:
    void show_main_page() {
        cout << "Welcome! you can enter 'logout' at anytime for returning to this page." << endl;
        cout << "enter 'exit' in this page for finishing the program." << endl;
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
    }

private:
    void execute_command(string command) {
        if (command == "login") {
            handle_login();
        }
        if (command == "signup") {
            handle_signup();
        }
    }

    void handle_login() {
        cout << "in which role do you want to login? ('student'/'professor')" << endl;
        string role;
        getline(cin, role);
        string password, id;
        cout << "enter your username" << endl;
        getline(cin, id);
        cout << "enter your password" << endl;
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

    void validate_login_data(string role, string id, string password) {
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
    }

    void handle_signup() {
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

    void student_signup() {
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
unordered_map <string, Exam_response*> Exam_response::id_to_pointer;
int Exam_response::instance_count = 0;



int main() {
    // freopen("in.txt", "r", stdin);
    Authentication_CLI *command_line_interface = new Authentication_CLI();
    command_line_interface->show_main_page();

    ofstream file("professor.json", ios::app); //to append data at end of last file

    for (auto i : Trackable_object<Professor>::instance_list) {
        file << i->to_json().dump(4) << endl;
    }

    file.close();

    ofstream fff("student.json", ios::app); //to append data at end of last file

    for (auto i : Trackable_object<Student>::instance_list) {
        fff << i->to_json().dump(4) << endl;
    }

    fff.close();

    ofstream f("exam.json", ios::app); //to append data at end of last file

    for (auto i : Trackable_object<Exam>::instance_list) {
        f << i->to_json().dump(4) << endl;
    }

    f.close();

    ofstream ff("exam_response.json", ios::app); //to append data at end of last file

    for (auto i : Trackable_object<Exam_response>::instance_list) {
        ff << i->to_json().dump(4) << endl;
    }

    ff.close();

}