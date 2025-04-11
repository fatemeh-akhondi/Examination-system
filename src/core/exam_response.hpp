#ifndef EXAM_RESPONSE_HPP
#define EXAM_RESPONSE_HPP

#include <vector>
#include <string>
#include <unordered_map>
#include "question.hpp"
#include "../../Lib/json.hpp"
#include "instance_manager.hpp"

using json = nlohmann::json;
using namespace std;

struct Question_response {
    Question* question;
    std::string answer;
    std::string correct_answer;
    int score;
};

class Exam_response : public Instance_manager<Exam_response> {
public:
    vector <Question_response> question_responses;

    Exam_response(string submitter_id, int exam_id, vector <Question*> questions);
    Exam_response(int id, string submitter_id, int exam_id, float score,
         vector <Question_response> question_responses);

    float get_score() { calculate_score(); return score; }
    int get_id() { return id; }
    string get_submitter_id() { return submitter_id; }
    int get_exam_id() { return exam_id; }

    static Exam_response* get_exam_response(int id);
    void calculate_score();
    void build_response_text(string file_path);

    json to_json();
    static void from_json(json &j);

private:
    static unordered_map <int, Exam_response*> id_to_pointer;
    static int instance_count;
    int id;
    string submitter_id;
    int exam_id;
    float score = 0;
};

#endif