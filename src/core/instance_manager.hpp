#ifndef INSTANCE_MANAGER_HPP
#define INSTANCE_MANAGER_HPP

#include <unordered_set>

template <typename T>
class Instance_manager {
public:
    Instance_manager();

    static std::unordered_set <T*> get_instance_list() { return instance_list; }

    static void delete_all();
    ~Instance_manager();
private:
    static std::unordered_set <T*> instance_list;
};

#endif
