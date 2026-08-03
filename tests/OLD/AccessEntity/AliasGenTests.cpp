#include <gtest/gtest.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <AccessEntity/AccessGenerator.h>

using namespace cg::build;
using namespace cg::generate;

TEST(AliasGenTest, Dependent) {
    auto attr_t = cg::build::TypeBuilder("AttributeVectorT")
        .as_template()
        .build();
    auto data_t = cg::build::TypeBuilder("data_type")
        .ns(attr_t)
        .build();

    auto data_alias = AliasBuilder("DataType")
        .with_template(attr_t)
        .underlying_type(data_t)
        .build();

	EXPECT_EQ(AliasGenerator::generate(data_alias),
        "template<typename AttributeVectorT>\nusing DataType = typename AttributeVectorT::data_type;");
}

TEST(AliasGenTest, Template) {
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

    auto PointerType = AliasBuilder("PointerType")
        .underlying_type(cond_t)
        .build();

	EXPECT_EQ(AliasGenerator::generate(PointerType),
        "using PointerType = std::conditional_t<IsConst, const DataType*, DataType*>;");
}

TEST(AliasGenTest, DependentTemplate) {
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

	auto vec_type_alias = AliasBuilder("vec_type")
        .with_template(attr_t)
        .with_template(tag_t)
        .underlying_type(vec_t)
        .build();

	EXPECT_EQ(AliasGenerator::generate(vec_type_alias),
        "template<typename AttributeVectorT, typename Tag>\nusing vec_type = typename AttributeVectorT::template vec_type<typename Tag::type>;");
}