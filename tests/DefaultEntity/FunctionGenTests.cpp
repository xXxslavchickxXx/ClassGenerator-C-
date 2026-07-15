#include <gtest/gtest.h>
#include <ClassGen.h>

using namespace cg::build;
using namespace cg::generate;
using namespace cg::source;

TEST(FunctionGeneratorTest, RegularFunction) {
    auto func = FunctionBuilder("foo")
        .with_type(TypeBuilder("void").build())
        .build();

    // Теперь декларация имеет точку с запятой в конце!
    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "void foo();"
    );

    // Тело функции начинается с пробела " {" вместо переноса строки
    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "void foo() {\n\t//TODO...\n}"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Inline),
        "void foo() {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, FunctionWithReturnType) {
    auto func = FunctionBuilder("get_value")
        .with_type(TypeBuilder("int").build())
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "int get_value();"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "int get_value() {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, FunctionWithArguments) {
    auto func = FunctionBuilder("add")
        .with_type(TypeBuilder("int").build())
        .add_argument(VariableBuilder("a").with_type(TypeBuilder("int").build()).build())
        .add_argument(VariableBuilder("b").with_type(TypeBuilder("int").build()).build())
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "int add(int a, int b);"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "int add(int a, int b) {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, FunctionWithDefaultArguments) {
    auto func = FunctionBuilder("multiply")
        .with_type(TypeBuilder("int").build())
        .add_argument(VariableBuilder("a")
            .with_type(TypeBuilder("int").build())
            .with_value("2")
            .build())
        .add_argument(VariableBuilder("b")
            .with_type(TypeBuilder("int").build())
            .with_value("3")
            .build())
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "int multiply(int a = 2, int b = 3);"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "int multiply(int a, int b) {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, StaticFunction) {
    auto func = FunctionBuilder("static_foo")
        .with_type(TypeBuilder("void").build())
        .as_static()
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "static void static_foo();"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "void static_foo() {\n\t//TODO...\n}"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Inline),
        "static void static_foo() {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, InlineFunction) {
    auto func = FunctionBuilder("inline_foo")
        .with_type(TypeBuilder("void").build())
        .as_inline()
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "inline void inline_foo() {\n\t//TODO...\n}"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        ""
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Inline),
        "inline void inline_foo() {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, ConstexprFunction) {
    auto func = FunctionBuilder("square")
        .with_type(TypeBuilder("int").build())
        .as_constexpr()
        .add_argument(VariableBuilder("x").with_type(TypeBuilder("int").build()).build())
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "constexpr int square(int x) {\n\t//TODO...\n}"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        ""
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Inline),
        "constexpr int square(int x) {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, FunctionWithNamespace) {
    auto func = FunctionBuilder("bar")
        .with_type(TypeBuilder("void").build())
        .ns(NamespacePrefix("myapp"))
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "void myapp::bar();"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "void myapp::bar() {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, TemplateFunction) {
    auto func = FunctionBuilder("max")
        .with_type(TypeBuilder("T").build())
        .with_template(TypeBuilder("T").as_template().build())
        .add_argument(VariableBuilder("a").with_type(TypeBuilder("T").build()).build())
        .add_argument(VariableBuilder("b").with_type(TypeBuilder("T").build()).build())
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "template<typename T>\nT max(T a, T b);"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "template<typename T>\ninline T max(T a, T b) {\n\t//TODO...\n}"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Inline),
        "template<typename T>\nT max(T a, T b) {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, TemplateFunctionWithDefaultArgs) {
    auto func = FunctionBuilder("multiply")
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

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "template<typename T>\nT multiply(T a = 2, T b = 3);"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "template<typename T>\ninline T multiply(T a, T b) {\n\t//TODO...\n}"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Inline),
        "template<typename T>\nT multiply(T a = 2, T b = 3) {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, FunctionWithConstArgument) {
    auto func = FunctionBuilder("copy")
        .with_type(TypeBuilder("void").build())
        .add_argument(VariableBuilder("src")
            .with_type(TypeBuilder("int").as_const().build())
            .build())
        .add_argument(VariableBuilder("dst")
            .with_type(TypeBuilder("int").as_ref().build())
            .build())
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "void copy(const int src, int& dst);"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "void copy(const int src, int& dst) {\n\t//TODO...\n}"
    );
}

TEST(FunctionGeneratorTest, StaticFunctionWithNamespace) {
    auto func = FunctionBuilder("helper")
        .with_type(TypeBuilder("void").build())
        .as_static()
        .ns(NamespacePrefix("utils"))
        .build();

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Declaration),
        "static void utils::helper();"
    );

    EXPECT_EQ(
        FunctionGenerator::generate(func, GenStage::Realization),
        "void utils::helper() {\n\t//TODO...\n}"
    );
}