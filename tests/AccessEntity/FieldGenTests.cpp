#include <gtest/gtest.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <AccessEntity/AccessGenerator.h>

using namespace cg::build;
using namespace cg::generate;

TEST(FieldGeneratorTest, NormalMemberField) {
    auto field = FieldBuilder("m_age")
        .with_type(TypeBuilder("int").build())
        .build();

    auto cls = ClassBuilder("Person").build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "int m_age;");
    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Realization), "");
    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Inline), "int m_age;");
}

TEST(FieldGeneratorTest, MemberFieldWithInitializer) {
    auto field = FieldBuilder("m_score")
        .with_type(TypeBuilder("double").build())
        .with_value("100.0")
        .build();

    auto cls = ClassBuilder("Player").build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "double m_score = 100.0;");
    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Realization), "");
}

TEST(FieldGeneratorTest, MutableField) {
    auto field = FieldBuilder("m_mutex")
        .with_type(TypeBuilder("mutex").ns("std").build())
        .as_mutable()
        .build();

    auto cls = ClassBuilder("SafeQueue").build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "mutable std::mutex m_mutex;");
}

TEST(FieldGeneratorTest, ClassicStaticField) {
    auto field = FieldBuilder("s_instances")
        .with_type(TypeBuilder("size_t").build())
        .with_value("0")
        .as_static()
        .build();

    auto cls = ClassBuilder("Widget").build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "static size_t s_instances = 0;");

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Realization), "size_t Widget::s_instances = 0;");
}

TEST(FieldGeneratorTest, StaticConstexprField) {
    auto field = FieldBuilder("MAX_LIMIT")
        .with_type(TypeBuilder("int").build())
        .with_value("1000")
        .as_static()
        .as_constexpr()
        .build();

    auto cls = ClassBuilder("Config").build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "static constexpr int MAX_LIMIT = 1000;");

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Realization), "");
}

TEST(FieldGeneratorTest, StaticInlineField) {
    auto field = FieldBuilder("s_logger_name")
        .with_type(TypeBuilder("string").ns("std").build())
        .with_value("\"Core\"")
        .as_static()
        .as_inline()
        .build();

    auto cls = ClassBuilder("Logger").build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "static inline std::string s_logger_name = \"Core\";");
    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Realization), "");
}

TEST(FieldGeneratorTest, StaticFieldInTemplateClass) {
    auto field = FieldBuilder("s_default_value")
        .with_type(TypeBuilder("T").build())
        .as_static()
        .build();

    auto cls = ClassBuilder("Repository")
        .with_template(TypeBuilder("T").as_template().build())
        .build();

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Declaration), "static T s_default_value;");

    std::string expected_realization =
        "template<typename T>\n"
        "T Repository<T>::s_default_value;";

    EXPECT_EQ(FieldGenerator::generate(field, cls, GenStage::Realization), expected_realization);
}