#pragma once

#include <ClassGen.h>

inline cg::source::Class build_class_in_class() {
    using namespace cg::build;

    auto t_t = TypeBuilder("T")
        .as_variadic()
        .build();
    auto u_t = TypeBuilder("U")
        .as_template()
        .build();
    // Внешний класс
    return ClassBuilder("Outer")
        .with_template(u_t)
        .with_template(t_t)
        .with_field(FieldBuilder("x").with_type(TypeBuilder("int").build()).as_private().build())
        .with_field(FieldBuilder("u").with_type(u_t).as_private().build())
        .with_class(ClassBuilder("Inner")
            .with_field(FieldBuilder("y").with_type(TypeBuilder("float").build()).as_public().build())
            .as_public()
            .build())
        .build();
}

