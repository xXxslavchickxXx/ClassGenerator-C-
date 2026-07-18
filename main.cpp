#include <ClassGen.h>
#include <iostream>

using namespace cg::build;
using namespace cg::generate;

cg::source::Class build_base_proxy();
auto build_vec_type() -> cg::source::TypeName;
auto build_pointer_type() -> cg::source::TypeName;
auto build_data_type() -> cg::source::TypeName;

int main() {
    system("chcp 65001 > nul");

    std::cout << ClassGenerator::generate(build_base_proxy(), GenStage::Declaration);

    return 0;
}

cg::source::Class build_base_proxy() {
    using namespace cg::build;

    auto attr_t = TypeBuilder("AttributeVectorT")
        .as_template()
        .build();

    auto const_t = TypeBuilder("IsConst")
        .as_template(TypeBuilder("bool").build())
        .build();

    auto tags_t = TypeBuilder("SelectedTags")
        .as_template()
        .as_variadic()
        .build();

    auto tag_t = TypeBuilder("Tag")
        .as_template()
        .build();

    auto vec_type = TypeBuilder("vec_type")
        .with_template(tag_t)
        .build();

    auto pointer_type = TypeBuilder("PointerType")
        .build();

    auto cls = ClassBuilder("base_proxy")
        .as_public()
        .with_template(attr_t)
        .with_template(const_t)
        .with_template(tags_t)
        .build();

    // Временное решение — добавим как поля, потом заменим на алиасы
    auto data_type_alias = AliasBuilder("DataType")
        .underlying_type(
            TypeBuilder("data_type")
            .ns(cg::source::NamespacePrefix("AttributeVectorT"))
            .build()
        )
        .as_public()
        .build();
    cls.add_alias(data_type_alias);

    auto pointer_type_alias = AliasBuilder("PointerType")
        .underlying_type(
            build_pointer_type()
        )
        .as_public()
        .build();
    cls.add_alias(pointer_type_alias);

    auto data_field = FieldBuilder("data_")
        .with_type(
            TypeBuilder("PointerType")
            .as_ptr()
            .build()
        )
        .as_protected()
        .build();
    cls.add_field(data_field);

    auto ctor = ConstructorBuilder()
        .as_public()
        .add_argument(
            VariableBuilder("data")
            .with_type(
                TypeBuilder("PointerType")
                .as_ptr()
                .build()
            )
            .build()
        )
        .build();
    cls.add_constructor(ctor);

    auto vec_method = MethodBuilder("vector")
        .with_type(
            TypeBuilder(vec_type)
            .as_const()
            .as_ref()
            .build()
        )
        .as_public()
        .with_template(tag_t)
        .build();
    cls.add_method(vec_method);

    auto mut_vec_method = MethodBuilder("mutable_vector")
        .with_type(
            TypeBuilder(vec_type)
            .as_ref()
            .build()
        )
        .as_public()
        .with_template(tag_t)
        .build();
    cls.add_method(mut_vec_method);

    auto size_method = MethodBuilder("size")
        .with_type(TypeBuilder("size_t").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(size_method);

    auto capacity_method = MethodBuilder("capacity")
        .with_type(TypeBuilder("size_t").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(capacity_method);

    auto empty_method = MethodBuilder("empty")
        .with_type(TypeBuilder("bool").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(empty_method);

    auto idx_method = MethodBuilder("get_tag_index")
        .with_type(TypeBuilder("size_t").build())
        .as_protected()
        .as_constexpr()
        .as_const()
        .with_template(
            TypeBuilder("Tag")
            .as_template()
            .build()
        )
        .build();
    cls.add_method(idx_method);

    auto call_method = MethodBuilder("call")
        .with_type(TypeBuilder("void").build())
        .as_protected()
        .as_constexpr()
        .as_inline()
        .with_template(
            TypeBuilder("Tag")
            .as_template()
            .build()
        )
        .with_template(
            TypeBuilder("F")
            .as_template()
            .build()
        )
        .add_argument(
            VariableBuilder("f")
            .with_type(
                TypeBuilder("F")
                .as_uni_ref()
                .build()
            )
            .build()
        )
        .build();
    cls.add_method(call_method);

    return cls;
}

auto build_vec_type() -> cg::source::TypeName {
    auto tag_t = cg::build::TypeBuilder("Tag")
        .as_template()
        .build();
    auto type_t = cg::build::TypeBuilder("type")
        .ns(tag_t)
        .build();
    auto attr_t = cg::build::TypeBuilder("AttributeVectorT")
        .as_template()
        .build();
    auto vec_t = cg::build::TypeBuilder("vec_type")
        .with_template(type_t)
        .ns(attr_t)
        .build();

    return vec_t;
}

auto build_pointer_type() -> cg::source::TypeName {
    auto Data_t = cg::build::TypeBuilder("DataType")
        .as_ptr()
        .build();
    auto c_Data_t = cg::build::TypeBuilder(Data_t)
        .as_const()
        .build();

    auto IsConst_t = cg::build::TypeBuilder("IsConst")
        .as_template(cg::build::TypeBuilder("bool").build())
        .build();

    auto cond_t = cg::build::TypeBuilder("conditional_t")
        .with_template(IsConst_t)
        .with_template(c_Data_t)
        .with_template(Data_t)
        .ns("std")
        .build();

    return cond_t;
}

auto build_data_type() -> cg::source::TypeName {
    auto attr_t = cg::build::TypeBuilder("AttributeVectorT")
        .as_template()
        .build();
    auto data_t = cg::build::TypeBuilder("data_type")
        .ns(attr_t)
        .build();

    return data_t;
}