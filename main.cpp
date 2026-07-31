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

    auto t_t = TypeBuilder("T")
        .as_variadic()
        .build();
    auto u_t = TypeBuilder("U")
        .as_variadic()
        //.as_template()
        .build();

    // Внешний класс
    auto outer = ClassBuilder("Outer")
        .with_template(t_t)
        .with_template(u_t)
        .with_field(FieldBuilder("x").with_type(TypeBuilder("int").build()).as_private().build())
        .with_field(FieldBuilder("t").with_type(t_t).as_private().build())
        .with_class(ClassBuilder("Inner")
            .with_field(FieldBuilder("y").with_type(TypeBuilder("float").build()).as_public().build())
            .as_public()
            .build())
        .build();

    cg::validate::ClassValidator cv;
    cv.verify(outer);

    // Генерация с правильными отступами:
    std::cout << ClassGenerator::generate(outer, GenStage::Declaration);

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
