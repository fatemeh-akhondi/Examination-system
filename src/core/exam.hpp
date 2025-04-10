#ifndef EXAM_HPP
#define EXAM_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "question.hpp"
#include "../../Lib/json.hpp"
#include "instance_manager.hpp"
#include "exam_response.hpp"

using json = nlohmann::json;
using namespace std;

class Exam: public Instance_manager<Exam> {
public:
    Exam(string name, int time_limit);
    Exam(string name, int time_limit, int id,
         vector <Question*> questions, vector <Exam_response*> responses);

    
    int get_unique_code() {  return id; }
    vector <Question*> get_questions() { return questions; }
    int get_id() { return id; }
    int get_time_limit() { return time_limit; }
    string get_name() { return name; }
    
    static Exam* get_exam(int id);

    void add_question(Question* new_question);
    void remove_question(int question_id);
    void print_exam();
    void prepare_questions();

    json to_json();
    static void from_json(json &j);
private:
    static unordered_map <int, Exam*> id_to_pointer;
    string name;
    int time_limit;
    int id;
    vector <Question*> questions;
    vector <Exam_response*> responses;

    int generate_unique_code();
};
    

#endif // EXAM_HPP