#ifndef AUTHENTICATION_CLI_HPP
#define AUTHENTICATION_CLI_HPP

#include "../core/student.hpp"
#include "../core/professor.hpp"
#include "student_cli.hpp"
#include "professor_cli.hpp"
#include "../utils/tools.hpp"
#include <string>

class Authentication_CLI {
public:
    void show_main_page();

private:
    void execute_command(std::string command);
    void handle_login();
    void handle_signup();
    void validate_login_data(std::string role, std::string id, std::string password);
    void professor_signup();
    void student_signup();
};

#endif // AUTHENTICATION_CLI_HPP