#include <gtest/gtest.h>
#include <AccessEntity/AccessEntity.h>
#include <AccessEntity/AccessBuilder.h>
#include <AccessEntity/AccessGenerator.h>

using namespace cg::build;
using namespace cg::generate;

TEST(MethodGeneratorTest, SimpleMethod) {
    auto method = MethodBuilder("update")
        .with_type(TypeBuilder("void").build())
        .build();

    auto cls = ClassBuilder("Engine").build();

    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Declaration), "void update();");

    std::string expected_realization =
        "void Engine::update() {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Realization), expected_realization);
}

TEST(MethodGeneratorTest, ConstMethodWithArguments) {
    auto method = MethodBuilder("get_distance")
        .with_type(TypeBuilder("double").build())
        .add_argument(VariableBuilder("target")
            .with_type(TypeBuilder("Point").as_const().as_ref().build())
            .build())
        .as_const()
        .build();

    auto cls = ClassBuilder("Navigator").build();

    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Declaration),
        "double get_distance(const Point& target) const;");

    std::string expected_realization =
        "double Navigator::get_distance(const Point& target) const {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Realization), expected_realization);
}

TEST(MethodGeneratorTest, VirtualMethods) {
    auto cls = ClassBuilder("IDrawable").build();

    auto draw_method = MethodBuilder("draw")
        .with_type(TypeBuilder("void").build())
        .as_abstract()
        .build();

    EXPECT_EQ(MethodGenerator::generate(draw_method, cls, GenStage::Declaration),
        "virtual void draw() = 0;");

    EXPECT_EQ(MethodGenerator::generate(draw_method, cls, GenStage::Realization), "");
    EXPECT_EQ(MethodGenerator::generate(draw_method, cls, GenStage::Inline),
        "virtual void draw() = 0;");
}

TEST(MethodGeneratorTest, VirtualAndOverrideMethods) {
    auto cls = ClassBuilder("Shape").build();

    auto area = MethodBuilder("area")
        .with_type(TypeBuilder("double").build())
        .as_const()
        .as_virtual()
        .build();

    EXPECT_EQ(MethodGenerator::generate(area, cls, GenStage::Declaration), "virtual double area() const;");

    std::string expected_area_realization =
        "double Shape::area() const {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(area, cls, GenStage::Realization), expected_area_realization);

    auto circle_cls = ClassBuilder("Circle").build();
    auto area_override = MethodBuilder("area")
        .with_type(TypeBuilder("double").build())
        .as_const()
        .as_override()
        .build();

    EXPECT_EQ(MethodGenerator::generate(area_override, circle_cls, GenStage::Declaration), "double area() const override;");

    std::string expected_override_realization =
        "double Circle::area() const {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(area_override, circle_cls, GenStage::Realization), expected_override_realization);
}

TEST(MethodGeneratorTest, DefaultAndDeletedMethods) {
    auto cls = ClassBuilder("NonCopyable").build();

    auto op_assign = MethodBuilder("operator=")
        .with_type(TypeBuilder("NonCopyable").as_ref().build())
        .add_argument(VariableBuilder("other").with_type(TypeBuilder("NonCopyable").as_const().as_ref().build()).build())
        .as_deleted()
        .build();

    EXPECT_EQ(MethodGenerator::generate(op_assign, cls, GenStage::Declaration),
        "NonCopyable& operator=(const NonCopyable& other) = delete;");

    EXPECT_EQ(MethodGenerator::generate(op_assign, cls, GenStage::Realization), "");
}

TEST(MethodGeneratorTest, StaticMethod) {
    auto method = MethodBuilder("create")
        .with_type(TypeBuilder("shared_ptr").ns("std")
        .with_template(TypeBuilder("Widget").build()).build())
        .as_static()
        .build();

    auto cls = ClassBuilder("WidgetFactory").build();

    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Declaration),
        "static std::shared_ptr<Widget> create();");

    std::string expected_realization =
        "std::shared_ptr<Widget> WidgetFactory::create() {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Realization), expected_realization);
}

TEST(MethodGeneratorTest, TemplateMethod) {
    auto method = MethodBuilder("cast")
        .with_type(TypeBuilder("U").build())
        .add_template_parametr(TypeBuilder("U").as_template().build())
        .build();

    auto cls = ClassBuilder("Converter").build();

    std::string expected_decl =
        "template<typename U>\n"
        "U cast();";
    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Declaration), expected_decl);

    std::string expected_realization =
        "template<typename U>\n"
        "U Converter::cast() {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Realization), expected_realization);
}

TEST(MethodGeneratorTest, MethodInTemplateClass) {
    auto method = MethodBuilder("push")
        .with_type(TypeBuilder("void").build())
        .add_argument(VariableBuilder("value").with_type(TypeBuilder("T").as_const().as_ref().build()).build())
        .build();

    auto cls = ClassBuilder("Stack")
        .add_template_parametr(TypeBuilder("T").as_template().build())
        .build();

    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Declaration), "void push(const T& value);");

    std::string expected_realization =
        "template<typename T>\n"
        "void Stack<T>::push(const T& value) {\n"
        "\t//TODO...\n"
        "}";
    EXPECT_EQ(MethodGenerator::generate(method, cls, GenStage::Realization), expected_realization);
}