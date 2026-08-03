#include <ClassGen.h>
#include <StructFileGen/StructFileManager.h>

#include <Test.h>

using namespace cg::build;
using namespace cg::generate;
namespace cgf = cg::file;
namespace fs = std::filesystem;

int main() { 
    system("chcp 65001 > nul");

    auto class_in_class = build_class_in_class();
    auto func_with_default = build_function_with_default();

    func_with_default.set_definition(
        cg::file::DEFINITION_TYPE::CROSSPLATOFORM);

    auto ns = cg::source::Namespace("my");
    ns.add_class(class_in_class);
    //ns.add_function(func_with_default);

    //std::cout << MethodGenerator::generate(method, cls);

    // Генерация с правильными отступами:
    std::cout << NamespaceGenerator::generate(ns);
    std::cout << "\n\n\nRealization:\n\n";
    std::cout << NamespaceGenerator::generate(ns, true);

    return 0;
}
/*auto path = fs::current_path();

    auto file_manager = cg::file::struct_file_manager("class_",
    path,
    "LibA",
    cgf::DEFINITION_TYPE::WINDOWS,
    cgf::DEFINITION_TYPE::LINUX | cgf::DEFINITION_TYPE::CROSSPLATOFORM);

    file_manager.create_structure();*/
    //std::cout << cg::file::open_geometry_path_dialog(path.string().c_str()) << "\n";
