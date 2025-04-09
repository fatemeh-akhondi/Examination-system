#ifndef INSTANCE_MANAGER_HPP
#define INSTANCE_MANAGER_HPP

#include <unordered_set>

template <typename T>
class Instance_manager {
public:
    Instance_manager();
    static void delete_all();
    ~Instance_manager();
private:
    static unordered_set <T*> instance_list;
};

#endif
