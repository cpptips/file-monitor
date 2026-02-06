#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

int main() {
    std::ifstream config_file("config.json");
    json j;
    config_file >> j;
    
    std::cout << "配置文件内容:" << std::endl;
    std::cout << j.dump(4) << std::endl;
    
    std::cout << "report_dir_sizes: " << j["report_dir_sizes"] << std::endl;
    std::cout << "类型: " << j["report_dir_sizes"].type_name() << std::endl;
    
    return 0;
}
