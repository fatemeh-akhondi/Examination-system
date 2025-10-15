#include "four_multiple_choice.hpp"
#include "constants.hpp"
#include "../utils/tools.hpp"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

Four_multiple_choice::Four_multiple_choice(string text, string answer, int positive_mark,
    int negative_mark, string opstions[4]): Question(text, answer, positive_mark, question_types::FMC) {
       this->negative_mark = negative_mark;

       for (int i = 0; i < 4; i++) {
           this->options[i] = opstions[i];
       }
}

Four_multiple_choice::Four_multiple_choice(int id, string text, string answer, int positive_mark,
    int negative_mark, string opstions[4]): Question(id, text, answer, positive_mark, question_types::FMC) {
       this->negative_mark = negative_mark;

       for (int i = 0; i < 4; i++) {
           this->options[i] = opstions[i];
       }
}

void Four_multiple_choice::shuffle_options() {
   int shuffled_order[4] = {1, 2, 3, 4};
   shuffle(shuffled_order, shuffled_order + 4, rng);

   //reorder options according to shuffled order
   string tmp[4];
   for (int i = 0; i < 4; i++) tmp[i] = options[i];
   for (int i = 0; i < 4; i++) options[i] = tmp[shuffled_order[i] - 1];
   
   //modify answer after reorder
   for (int i = 0; i < 4; i++) {
       if (to_string(shuffled_order[i]) == answer) {
           answer = to_string(i + 1);
           return;
       }
   }
}

void Four_multiple_choice::Four_multiple_choice::print_question(ostream& os) {
   os << text << endl;
   for (int i = 0; i < 4; i++) {
       os << i + 1 << ": " << options[i] << endl;
   }
   os << "Positive mark: " << positive_mark << " / Negative mark: " << negative_mark << endl;
}

json Four_multiple_choice::to_json() {
   json j = Question::to_json();
   j["options"] = json::array();
   for (int i = 0; i < 4; i++) {
       j["options"].push_back(options[i]);
   }

   return j;
}

void Four_multiple_choice::from_json(json& j) {
    std::string text = j["text"];
    std::string answer = j["answer"];
    int id = j["id"].get<int>();
    int positive_mark = j["positive_mark"].get<int>();
    int negative_mark = j["negative_mark"].get<int>();
    string options[4];
    for (int i = 0; i < 4; i++) {
        options[i] = j["options"][i];
    }
    new Four_multiple_choice(id, text, answer, positive_mark, negative_mark, options);
}