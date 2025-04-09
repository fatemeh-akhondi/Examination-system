#include "four_multiple_choice.hpp"
#include <iostream>
#include <algorithm>
#include <random>

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
using namespace std;

Four_multiple_choice::Four_multiple_choice(string text, string answer, int positive_mark,
    int negative_mark, string opstions[4]): Question(text, answer, positive_mark) {
       this->negative_mark = negative_mark;

       for (int i = 0; i < 4; i++) {
           this->options[i] = opstions[i];
       }
}

void Four_multiple_choice::shuffle_options() {
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

void Four_multiple_choice::Four_multiple_choice::print_question() {
   cout << text << endl;
   for (int i = 0; i < 4; i++) {
       cout << i + 1 << ": " << options[i] << endl;
   }
   cout << "positive mark: " << positive_mark << " / negative mark: " << negative_mark << endl;
}

json Four_multiple_choice::to_json() {
   json j = Question::to_json();
   j["options"] = json::array();
   for (int i = 0; i < 4; i++) {
       j["options"].push_back(options[i]);
   }

   return j;
}