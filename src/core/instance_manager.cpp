#include "instance_manager.hpp"
#include <vector>

#include "question.hpp"
#include "exam.hpp"
#include "exam_response.hpp"
#include "professor.hpp"
#include "student.hpp"

template class Instance_manager<Question>;
template class Instance_manager<Exam>;
template class Instance_manager<Exam_response>;
template class Instance_manager<Professor>;
template class Instance_manager<Student>;


template <typename T>
std::unordered_set<T*> Instance_manager<T>::instance_list;

template <typename T>
Instance_manager<T>::Instance_manager() {
    instance_list.insert(static_cast<T*>(this));
}

template <typename T>
Instance_manager<T>::~Instance_manager() {
    instance_list.erase(static_cast<T*>(this));
}

template <typename T>
void Instance_manager<T>::delete_all() {
    std::vector<T*> temporary_list;
    for (auto current : instance_list) {
        temporary_list.push_back(current);
    }

    for (T* instance : temporary_list) {
        delete instance;
    }
}