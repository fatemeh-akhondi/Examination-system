#include "exam_cli.hpp"
#include <iostream>
#include <iomanip>

using namespace std;

Exam_CLI::Exam_CLI(Exam* current_exam) {
    this->current_exam = current_exam;
}

Exam_response* Exam_CLI::hold_exam(string submitter_id) {
    cout << "press '>' for next question and '<' for previous question and 'end' for finishing the exam" << endl;
    cout << "you have " << current_exam->get_time_limit() << " minutes" << endl;

    time_t start_time;
    time(&start_time);

    Exam_response *response = process_exam_input(submitter_id, current_exam, start_time);

    cout << "exam finished! you have to wait for long-answer questions to get graded by professor." << endl;
    response->calculate_score();
    cout << "your score for this exam is: " << fixed << setprecision(2) << response->get_score() << endl;
    return response;
}

Exam_response* Exam_CLI::process_exam_input(string submitter_id, Exam* current_exam, time_t start_time) {
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

void Exam_CLI::display_response_data(int index, Question* question, Question_response& question_response) {
    cout << "question" << index + 1 << ": ";
    question->print_question();

    question_response.question = question; //TODO remove if exam in exam response stays

    if (question_response.answer != "") {
        cout << "your answer: " << question_response.answer << endl;
        cout << "if you want to edit your answer enter it" << endl;
    }
}

bool Exam_CLI::exam_ended(time_t &start_time, string &input) {
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

bool Exam_CLI::move_between_question(string &input, int &index, int last_index) {
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
