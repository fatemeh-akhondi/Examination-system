#ifndef EXAM_CLI_HPP
#define EXAM_CLI_HPP

#include "../core/exam.hpp"
#include "../core/exam_response.hpp"
#include "../utils/tools.hpp"

class Exam_CLI {
public:
    Exam_CLI(Exam* current_exam);

    Exam_response* hold_exam(std::string submitter_id);

private:
    Exam* current_exam;

    Exam_response* process_exam_input(std::string submitter_id, Exam* current_exam, time_t start_time);
    void display_response_data(int index, Question* question, Question_response& question_response);
    bool exam_ended(time_t& start_time, std::string& input);
    bool move_between_question(std::string& input, int& index, int last_index);
};

#endif // EXAM_CLI_HPP