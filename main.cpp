#include <ClassGen.h>
#include <StructFileGen/StructFileManager.h>

#include <iostream>
#include <filesystem>

using namespace cg::build;
using namespace cg::generate;
namespace cgf = cg::file;
namespace fs = std::filesystem;

int main() {
    system("chcp 65001 > nul");

    /*auto path = fs::current_path();

    auto file_manager = cg::file::struct_file_manager("class_",
    path,
    "LibA",
    cgf::DEFINITION_TYPE::WINDOWS,
    cgf::DEFINITION_TYPE::LINUX | cgf::DEFINITION_TYPE::CROSSPLATOFORM);

    file_manager.create_structure();*/
    //std::cout << cg::file::open_geometry_path_dialog(path.string().c_str()) << "\n";

    return 0;
}