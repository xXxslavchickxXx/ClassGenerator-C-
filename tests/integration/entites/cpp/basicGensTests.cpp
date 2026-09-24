#include <gtest/gtest.h>

#include <generation/generators/cpp/basic_generators.h>

#include <core/base/node/Node.h>
#include <core/entities/cpp/basic_entites.h>
#include <core/entities/EntityFabricator.h>

TEST(NamedEntityGeneratorIntegrationTest, ReturnsEmptyWhenNoEntity) {
    cg::core::Node node;
    cg::NamedEntityGenerator gen;
    EXPECT_EQ(gen.generate(&node), "");
}

TEST(NamedEntityGeneratorIntegrationTest, GenerateNameFromEntity) {
    std::string node_name = "word";

    auto ent = cg::entities::cpp::NamedEntity(node_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, cg::entities::cpp::NamedEntity>>(ent);

    auto gen = cg::NamedEntityGenerator();

    EXPECT_EQ(gen.generate(node.get()), node_name);
}