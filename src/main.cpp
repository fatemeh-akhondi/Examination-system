#include "communication/authentication_cli.hpp"
#include "data_exporter/data_exporter.hpp"
#include "data_loader/data_loader.hpp"

int main() {
    // freopen("in.txt", "r", stdin);
    Data_loader data_loader;
    data_loader.load_all_data();

    Authentication_CLI command_line_interface;
    command_line_interface.show_main_page();

    Data_exporter data_exporter;
    data_exporter.save_all_data();
}