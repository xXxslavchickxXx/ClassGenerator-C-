#include <ClassGen.h>
#include <iostream>

cg::source::Class build_base_proxy() {
    using namespace cg::build;

    // ============================================================
    // 1. Шаблонные параметры
    // ============================================================
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

    // ============================================================
    // 2. Типы для алиасов
    // ============================================================

    // vec_type = typename AttributeVectorT::template vec_type<typename Tag::type>
    auto type_of_tag = TypeBuilder("type")
        .ns(tag_t)  // Tag::type
        .build();

    auto vec_type = TypeBuilder("vec_type")
        .as_template()
        .with_template(type_of_tag)
        .ns(attr_t)  // AttributeVectorT::vec_type
        .build();

    // DataType = typename AttributeVectorT::data_type
    auto data_type = TypeBuilder("data_type")
        .ns(attr_t)
        .build();

    // PointerType = std::conditional_t<IsConst, const DataType*, DataType*>
    auto data_ptr = TypeBuilder("DataType")
        .as_ptr()
        .build();
    auto const_data_ptr = TypeBuilder("DataType")
        .as_const()
        .as_ptr()
        .build();
    auto is_const_type = TypeBuilder("IsConst")
        .build();

    auto pointer_type = TypeBuilder("conditional_t")
        .ns(cg::source::NamespacePrefix("std"))
        .with_template(is_const_type)
        .with_template(const_data_ptr)
        .with_template(data_ptr)
        .build();

    // ============================================================
    // 3. Класс base_proxy
    // ============================================================
    auto cls = ClassBuilder("base_proxy")
        .as_public()
        .add_template_parametr(attr_t)
        .add_template_parametr(const_t)
        .add_template_parametr(tags_t)
        .build();

    // ============================================================
    // 4. Алиасы
    // ============================================================

    // using vec_type = typename AttributeVectorT::template vec_type<typename Tag::type>;
    auto vec_type_alias = AliasBuilder("vec_type")
        .underlying_type(vec_type)
        .as_public()
        .build();
    cls.add_alias(vec_type_alias);

    // using DataType = typename AttributeVectorT::data_type;
    auto data_type_alias = AliasBuilder("DataType")
        .underlying_type(data_type)
        .as_public()
        .build();
    cls.add_alias(data_type_alias);

    // using PointerType = std::conditional_t<IsConst, const DataType*, DataType*>;
    auto pointer_type_alias = AliasBuilder("PointerType")
        .underlying_type(pointer_type)
        .as_public()
        .build();
    cls.add_alias(pointer_type_alias);

    // ============================================================
    // 5. Поле data_
    // ============================================================
    auto data_field = FieldBuilder("data_")
        .with_type(
            TypeBuilder(pointer_type_alias)
            .as_ptr()
            .build()
        )
        .as_protected()
        .build();
    cls.add_field(data_field);

    // ============================================================
    // 6. Конструкторы
    // ============================================================

    // base_proxy(PointerType data);
    auto ctor1 = ConstructorBuilder()
        .as_public()
        .add_argument(
            VariableBuilder("data")
            .with_type(
                TypeBuilder(pointer_type_alias)
                .as_ptr()
                .build()
            )
            .build()
        )
        .build();
    cls.add_constructor(ctor1);

    // base_proxy(DataType& data);
    auto ctor2 = ConstructorBuilder()
        .as_public()
        .add_argument(
            VariableBuilder("data")
            .with_type(
                TypeBuilder(data_type_alias)
                .as_ref()
                .build()
            )
            .build()
        )
        .build();
    cls.add_constructor(ctor2);

    // base_proxy(const DataType& data);
    auto ctor3 = ConstructorBuilder()
        .as_public()
        .add_argument(
            VariableBuilder("data")
            .with_type(
                TypeBuilder(data_type_alias)
                .as_const()
                .as_ref()
                .build()
            )
            .build()
        )
        .build();
    cls.add_constructor(ctor3);

    // ============================================================
    // 7. Методы
    // ============================================================

    // template<typename Tag> const vec_type<Tag>& vector() const;
    auto vec_method = MethodBuilder("vector")
        .with_type(
            TypeBuilder(vec_type_alias)
            .with_template(tag_t)
            .as_const()
            .as_ref()
            .build()
        )
        .as_public()
        .as_const()
        .add_template_parametr(tag_t)
        .build();
    cls.add_method(vec_method);

    // template<typename Tag> vec_type<Tag>& mutable_vector();
    auto mut_vec_method = MethodBuilder("mutable_vector")
        .with_type(
            TypeBuilder(vec_type_alias)
            .with_template(tag_t)
            .as_ref()
            .build()
        )
        .as_public()
        .add_template_parametr(tag_t)
        .build();
    cls.add_method(mut_vec_method);

    // size_t size() const;
    auto size_method = MethodBuilder("size")
        .with_type(TypeBuilder("size_t").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(size_method);

    // size_t capacity() const;
    auto capacity_method = MethodBuilder("capacity")
        .with_type(TypeBuilder("size_t").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(capacity_method);

    // bool empty() const;
    auto empty_method = MethodBuilder("empty")
        .with_type(TypeBuilder("bool").build())
        .as_public()
        .as_const()
        .build();
    cls.add_method(empty_method);

    // template<typename Tag> constexpr size_t get_tag_index() const;
    auto idx_method = MethodBuilder("get_tag_index")
        .with_type(TypeBuilder("size_t").build())
        .as_protected()
        .as_constexpr()
        .as_const()
        .add_template_parametr(
            TypeBuilder("Tag")
            .as_template()
            .build()
        )
        .build();
    cls.add_method(idx_method);

    // template<typename Tag, typename F> constexpr inline void call(F&& f);
    auto call_method = MethodBuilder("call")
        .with_type(TypeBuilder("void").build())
        .as_protected()
        .as_constexpr()
        .as_inline()
        .add_template_parametr(
            TypeBuilder("Tag")
            .as_template()
            .build()
        )
        .add_template_parametr(
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

int main() {
    auto cls = build_base_proxy();

    std::cout << "=== base_proxy CLASS ===\n";
    std::cout << cg::generate::ClassGenerator::generate(cls) << "\n";

    return 0;
}