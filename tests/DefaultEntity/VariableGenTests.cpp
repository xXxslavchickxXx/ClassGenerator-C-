#include <gtest/gtest.h>
#include <ClassGen.h>

using namespace cg::build;
using namespace cg::generate;

TEST(VariableGenerationTest, SimpleGlobalVariable) {
    auto var = VariableBuilder("global_counter")
        .with_type(TypeBuilder("int").build())
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "extern int global_counter;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), "int global_counter;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "int global_counter;");
}

TEST(VariableGenerationTest, GlobalVariableWithInit) {
    auto var = VariableBuilder("global_counter")
        .with_type(TypeBuilder("int").build())
        .with_value("10")
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "extern int global_counter = 10;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), "int global_counter = 10;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "int global_counter = 10;");
}

TEST(VariableGenerationTest, StaticVariable) {
    auto var = VariableBuilder("static_counter")
        .with_type(TypeBuilder("int").build())
        .as_static()
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "static int static_counter;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "static int static_counter;");
}

TEST(VariableGenerationTest, StaticConstexpr) {
    auto var = VariableBuilder("MAX_SIZE")
        .with_type(TypeBuilder("int").build())
        .with_value("100")
        .as_static()
        .as_constexpr()
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "static constexpr int MAX_SIZE = 100;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "static constexpr int MAX_SIZE = 100;");
}

TEST(VariableGenerationTest, StaticInline) {
    auto var = VariableBuilder("inline_counter")
        .with_type(TypeBuilder("int").build())
        .with_value("42")
        .as_inline()
        .as_static()
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "static inline int inline_counter = 42;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "static inline int inline_counter = 42;");
}

TEST(VariableGenerationTest, GlobalConstexpr) {
    auto var = VariableBuilder("PI")
        .with_type(TypeBuilder("float").build())
        .with_value("3.14159f")
        .as_constexpr()
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "constexpr float PI = 3.14159f;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "constexpr float PI = 3.14159f;");
}

TEST(VariableGenerationTest, StaticConstexprInit) {
    auto var = VariableBuilder("VERSION")
        .with_type(TypeBuilder("int").build())
        .with_value("1")
        .as_static()
        .as_constexpr()
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "static constexpr int VERSION = 1;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "static constexpr int VERSION = 1;");
}

TEST(VariableGenerationTest, StaticInlineConstexpr) {
    auto var = VariableBuilder("DEFAULT_VALUE")
        .with_type(TypeBuilder("int").build())
        .with_value("0")
        .as_constexpr()
        .as_inline()
        .as_static()
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "static constexpr inline int DEFAULT_VALUE = 0;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "static constexpr inline int DEFAULT_VALUE = 0;");
}

TEST(VariableGenerationTest, VariableInNamespace) {
    auto var = VariableBuilder("app_version")
        .with_type(TypeBuilder("int").build())
        .with_value("1")
        .ns(cg::source::NamespacePrefix("myapp"))
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "extern int myapp::app_version = 1;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), "int myapp::app_version = 1;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "int myapp::app_version = 1;");
}

TEST(VariableGenerationTest, ConstexprVariableInNamespace) {
    auto var = VariableBuilder("MAX_CONNECTIONS")
        .with_type(TypeBuilder("int").build())
        .with_value("1024")
        .as_constexpr()
        .ns(cg::source::NamespacePrefix("network"))
        .build();

    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Declaration), "constexpr int network::MAX_CONNECTIONS = 1024;");
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Realization), ""); // Пустая строка без ";"
    EXPECT_EQ(VariableGenerator::generate(var, GenStage::Inline), "constexpr int network::MAX_CONNECTIONS = 1024;");
}