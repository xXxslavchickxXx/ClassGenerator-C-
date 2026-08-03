#pragma once

#include <ClassGen.h>

/*auto path = fs::current_path();

auto file_manager = cg::file::struct_file_manager("class_",
path,
"LibA",
cgf::DEFINITION_TYPE::WINDOWS,
cgf::DEFINITION_TYPE::LINUX | cgf::DEFINITION_TYPE::CROSSPLATOFORM);

file_manager.create_structure();*/
//std::cout << cg::file::open_geometry_path_dialog(path.string().c_str()) << "\n";


inline cg::source::Class build_class_in_class() {
    using namespace cg::build;

    auto t_t = TypeBuilder("T")
        .as_variadic()
        .build();
    auto u_t = TypeBuilder("U")
        .as_template()
        .build();
    auto k_t = TypeBuilder("K")
        .as_template()
        .build();
    // Внешний класс
    return ClassBuilder("Outer")
        .with_template(t_t)
        .with_field(FieldBuilder("x").with_type(TypeBuilder("int").build()).as_private().build())
        .with_field(FieldBuilder("u").with_type(u_t).as_private().build())
        .with_class(ClassBuilder("Inner")
            .with_template(u_t)
            .with_field(FieldBuilder("y").with_type(TypeBuilder("float").build()).as_public().build())
            .with_method(MethodBuilder("method")
                .with_template(k_t)
                .with_realization()
                .add_argument(VariableBuilder("a")
                    .with_type(k_t)
                    .build())
                .build())
            .with_method(MethodBuilder("classic_method")
                .with_realization()
                .add_argument(VariableBuilder("i")
                    .with_type(TypeBuilder("int").build())
                    .build())
                .build())
            .as_public()
            .build())
        .build();
}

inline cg::source::Function build_function_with_default() {
    using namespace cg::build;

    return FunctionBuilder("multiply")
        .with_type(TypeBuilder("T").build())
        .with_template(TypeBuilder("T").as_template().build())
        .add_argument(VariableBuilder("a")
            .with_type(TypeBuilder("T").build())
            .with_value("2")
            .build())
        .add_argument(VariableBuilder("b")
            .with_type(TypeBuilder("T").build())
            .with_value("3")
            .build())
        .build();
}