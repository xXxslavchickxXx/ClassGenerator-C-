#include <ClassGen.h>
#include <FileGenerator/FileCreator.h>

#include <iostream>

using namespace cg::build;
using namespace cg::generate;

int main() {
    system("chcp 65001 > nul");

    std::cout << cg::file::open_geometry_path_dialog("test");

    return 0;
}