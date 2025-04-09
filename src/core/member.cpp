#include "member.hpp"

Member::Member(string name, string id, string password) {
    this->name = name;
    this->id = id;
    this->password = password;
}

json Member::to_json() {
    json j;
    j["name"] = name;
    j["id"] = id;
    j["password"] = password;

    return j;
}