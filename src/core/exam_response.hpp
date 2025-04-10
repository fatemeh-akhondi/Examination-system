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
};

class Exam_response : public Instance_manager<Exam_response> {
public:
    vector <Question_response> question_responses;

    Exam_response(string submitter_id, vector <Question*> questions);
    Exam_response(int id, string submitter_id, float score, vector <Question_response> question_responses);

    float get_score() { return score; }
    int get_id() { return id; }

    static Exam_response* get_exam_response(int id);
    void calculate_score();
    json to_json();
    static void from_json(json &j);

private:
    static unordered_map <int, Exam_response*> id_to_pointer;
    static int instance_count;
    int id;
    string submitter_id;
    float score = 0;
};

#endif