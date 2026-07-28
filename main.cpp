#include <ClassGen.h>
#include <FileGenerator/FileCreator.h>

#include <iostream>
#include <filesystem>

using namespace cg::build;
using namespace cg::generate;
namespace fs = std::filesystem;

int main() {
    system("chcp 65001 > nul");

    auto path = fs::current_path();

    std::cout << cg::file::open_geometry_path_dialog(path.string().c_str()) << "\n";

    return 0;
}