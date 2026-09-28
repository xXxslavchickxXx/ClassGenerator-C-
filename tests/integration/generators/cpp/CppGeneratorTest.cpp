#include <gtest/gtest.h>
#include <stubs.h>

#include <core/entities/cpp/entites.h>
#include <generation/generators/cpp/CppGenerators.h>
#include <generation/dispatchers/RegisteredDispatcher.h>

TEST(CPPGen, RelativeFunc) {
    // Empty Node
    EXPECT_EQ(cg::gen::cpp::relative_path(nullptr), "");

    auto main_name = cg::entities::cpp::NamedEntity("main");
    auto external_name = cg::entities::cpp::NamedEntity("external");

    // Creating main namespace and add external name into him
    auto main = cg::entities::cpp::Namespace(main_name);
    auto* ext_ptr = main.add_child(std::make_unique<cg::entities::cpp::Namespace>(external_name));

    EXPECT_EQ(cg::gen::cpp::relative_path(&main), "main");
    EXPECT_EQ(cg::gen::cpp::relative_path(ext_ptr), "main::external");
    EXPECT_EQ(cg::gen::cpp::relative_path(ext_ptr, &main), "external");

    // Path without NamedEntity (catching exception)
    auto node_without_entities = cg::core::Node();
    EXPECT_ANY_THROW(cg::gen::cpp::relative_path(&node_without_entities));
}

TEST(CPPGen, NamespaceGen) {
    auto main_name = cg::entities::cpp::NamedEntity("main");
    auto external_name = cg::entities::cpp::NamedEntity("external");

    // Creating main namespace and add external name into him
    auto main = cg::entities::cpp::Namespace(main_name);
    auto* ext_ptr = main.add_child(std::make_unique<cg::entities::cpp::Namespace>(external_name));

    auto dispatcher = cg::RegisteredDispatcher();

    dispatcher.add<cg::gen::cpp::NamespaceGenerator>();

    EXPECT_EQ(dispatcher.generate(&main), "namespace main {\n\n}");
    EXPECT_EQ(dispatcher.generate(ext_ptr), "namespace main::external {\n\n}");
    EXPECT_EQ(dispatcher.generate(ext_ptr, false, &main), "namespace external {\n\n}");
}