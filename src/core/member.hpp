#ifndef MEMBER_HPP
#define MEMBER_HPP

#include <string>
#include "../../Lib/json.hpp"

using json = nlohmann::json;
using namespace std;

class Member {
public:
    Member(string name, string id, string password);

    string get_password() { return password; }
    string get_id() { return id; }

    json to_json();

protected:
    string name;
    string id;
    string password;
};
    

#endif // MEMBER_HPP