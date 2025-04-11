#include "communication/authentication_cli.hpp"
#include "data_exporter/data_exporter.hpp"
#include "data_loader/data_loader.hpp"
#include "utils/data_cleaner.hpp"

int main() {
    Data_loader data_loader;
    data_loader.load_all_data();

    Authentication_CLI authentication_CLI;
    authentication_CLI.show_main_page();

    Data_exporter data_exporter;
    data_exporter.save_all_data();

    Data_cleaner data_cleaner;
    data_cleaner.delete_all_instances();
    return 0;
}