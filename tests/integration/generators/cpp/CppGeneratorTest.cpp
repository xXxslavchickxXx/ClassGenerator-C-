#include <gtest/gtest.h>
#include <stubs.h>

#include <core/entities/cpp/entites.h>
#include <generation/generators/cpp/CppGenerators.h>

TEST(CPPGen, RelativeFunc) {
    auto main_name = cg::entities::cpp::NamedEntity("main");
    auto external_name = cg::entities::cpp::NamedEntity("external");

    // Creating main namespace and add external name into him
    auto main = cg::entities::cpp::Namespace(main_name);
    auto* ext_ptr = main.add_child(std::make_unique<cg::entities::cpp::Namespace>(external_name));

    EXPECT_EQ(cg::gen::cpp::relative_path(&main), "main");
    EXPECT_EQ(cg::gen::cpp::relative_path(ext_ptr), "main::external");
    EXPECT_EQ(cg::gen::cpp::relative_path(ext_ptr, &main), "external");
}

TEST(CPPGen, NamespaceGen) {
    auto main_name = cg::entities::cpp::NamedEntity("main");
    auto external_name = cg::entities::cpp::NamedEntity("external");

    // Creating main namespace and add external name into him
    auto main = cg::entities::cpp::Namespace(main_name);
    auto* ext_ptr = main.add_child(std::make_unique<cg::entities::cpp::Namespace>(external_name));

    auto gen = cg::gen::cpp::NamespaceGenerator();

    // EXPECT_EQ(gen.generate(&main), "main");
    // EXPECT_EQ(gen.generate(ext_ptr), "main::external");
    // EXPECT_EQ(gen.generate(ext_ptr, false, &main), "external");
}