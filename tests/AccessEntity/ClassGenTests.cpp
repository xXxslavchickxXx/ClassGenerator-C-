#include <gtest/gtest.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <AccessEntity/AccessGenerator.h>

using namespace cg::build;
using namespace cg::generate;

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

cg::source::Class build_base_proxy() {
    auto attr_t = TypeBuilder("AttributeVectorT")
        .as_template(TypeBuilder("IsAttributeVector")
            .build()) // Шаблонный концепт
        .build();

    auto const_t = TypeBuilder("IsConst")
        .as_template(TypeBuilder("bool").build()) // bool IsConst
        .build();

    auto tags_t = TypeBuilder("SelectedTags")
        .as_variadic() // typename... SelectedTags
        .build();

    // Создаем сам класс и вешаем на него шаблоны
    auto cls = ClassBuilder("base_proxy")
        .with_template(attr_t)
        .with_template(const_t)
        .with_template(tags_t)
        .build();

    // Шаблонный параметр для алиаса vec_type
    auto tag_param = TypeBuilder("Tag")
        .as_template()
        .build();

    auto vec_t = build_vec_type();

    auto vec_type_alias = AliasBuilder("vec_type")
        .with_template(tag_param)
        .underlying_type(vec_t)
        .as_protected()
        .build();
    cls.add_alias(vec_type_alias);

    // using DataType = typename AttributeVectorT::data_type;
    auto data_t = build_data_type();

    auto data_type_alias = AliasBuilder("DataType")
        .underlying_type(data_t)
        .as_protected()
        .build();
    cls.add_alias(data_type_alias);

    // using PointerType = std::conditional_t<IsConst, const DataType*, DataType*>;
    auto pointer_t = build_pointer_type();

    auto pointer_type_alias = AliasBuilder("PointerType")
        .underlying_type(pointer_t)
        .as_protected()
        .build();
    cls.add_alias(pointer_type_alias);

    // using tags = std::tuple<SelectedTags...>;
    auto tags_alias = AliasBuilder("tags")
        .underlying_type(
            TypeBuilder("tuple").ns("std").with_template(TypeBuilder("SelectedTags...").build()).build()
        )
        .as_public()
        .build();
    cls.add_alias(tags_alias);

    // using owner_tags = typename AttributeVectorT::tags;
    auto owner_tags_alias = AliasBuilder("owner_tags")
        .underlying_type(
            TypeBuilder("tags")
            .ns(attr_t)
            .build()
        )
        .as_public()
        .build();
    cls.add_alias(owner_tags_alias);

    // PointerType data_;
    auto data_field = FieldBuilder("data_")
        .with_type(pointer_type_alias)
        .as_protected()
        .build();
    cls.add_field(data_field);

    // base_proxy(PointerType data);
    cls.add_constructor(ConstructorBuilder().as_public().add_argument(VariableBuilder("data").with_type(TypeBuilder("PointerType").build()).build()).build());
    // base_proxy(DataType& data);
    cls.add_constructor(ConstructorBuilder().as_public().add_argument(VariableBuilder("data").with_type(TypeBuilder("DataType").as_ref().build()).build()).build());
    // base_proxy(const DataType& data);
    cls.add_constructor(ConstructorBuilder().as_public().add_argument(VariableBuilder("data").with_type(TypeBuilder("DataType").as_ref().as_const().build()).build()).build());

    // template<typename Tag> const vec_type<Tag>& vector() const;
    auto vec_method = MethodBuilder("vector")
        .with_type(TypeBuilder(vec_type_alias)
            .as_const()
            .as_ref()
            .with_template(tag_param)
            .build()
        )
        .with_template(tag_param)
        .as_public()
        .as_const()
        .build();
    cls.add_method(vec_method);

    // size_t size() const;
    cls.add_method(MethodBuilder("size").with_type(TypeBuilder("size_t").build()).as_public().as_const().build());
    // size_t capacity() const;
    cls.add_method(MethodBuilder("capacity").with_type(TypeBuilder("size_t").build()).as_public().as_const().build());
    // bool empty() const;
    cls.add_method(MethodBuilder("empty").with_type(TypeBuilder("bool").build()).as_public().as_const().build());

    // template<typename Tag> vec_type<Tag>& mutable_vector();
    auto mut_vec_method = MethodBuilder("mutable_vector")
        .with_type(TypeBuilder(vec_type_alias)
            .with_template(tag_param)
            .as_ref()
            .build())
        .with_template(tag_param)
        .as_public()
        .build();
    cls.add_method(mut_vec_method);

    // template<typename Tag> constexpr size_t get_tag_index() const;
    auto idx_method = MethodBuilder("get_tag_index")
        .with_type(TypeBuilder("size_t").build())
        .with_template(tag_param)
        .as_protected()
        .as_constexpr()
        .as_const()
        .build();
    cls.add_method(idx_method);

    // template<typename Tag, typename F> constexpr void call(F&& f);
    auto t_f = TypeBuilder("F")
        .as_template()
        .build();
    auto call_method = MethodBuilder("call")
        .with_type(TypeBuilder("void").build())
        .with_template(tag_param)
        .with_template(t_f)
        .add_argument(VariableBuilder("f").with_type(TypeBuilder("F").as_uni_ref().build()).build())
        .as_protected()
        .as_constexpr()
        .build();
    cls.add_method(call_method);

    auto friend_stream_op = MethodBuilder("operator<<")
        .with_type(TypeBuilder("std::ostream&").build())
        .as_friend()
        .as_inline() // Определение прямо в теле класса
        .add_argument(VariableBuilder("os").with_type(TypeBuilder("std::ostream&").build()).build())
        .add_argument(VariableBuilder("proxy").with_type(TypeBuilder("base_proxy").as_ref().as_const().build()).build())
        .build();
    cls.add_method(friend_stream_op);

    return cls;
}

TEST(ClassGenTest, base_proxy) {
    auto cls = build_base_proxy();

    //EXPECT_EQ(ClassGenerator::generate(cls, GenStage::Declaration)
}

TEST(ClassGenTest, Declaration) {
	auto cls = ClassBuilder("Class").build();

	EXPECT_EQ(ClassGenerator::generate(cls, GenStage::Declaration), "class Class {\n};");
}