#include <gtest/gtest.h>
#include <stubs.h>

TEST(PriorityGenIntegrationTest, DeleteGenerator) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    auto gen = cg::PriorityGenerator();

    gen.take(1, std::make_unique<tests::AGen>());
    gen.take(2, std::make_unique<tests::BGen>());

    gen.release(2);

    EXPECT_EQ(gen.generate(node.get()), "5");
}

TEST(PriorityGenIntegrationTest, GenerateNestedGen) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    using NestedGenerator = cg::PriorityGeneratorFabricator<tests::BGen, tests::AGen>;

    auto gen = cg::PriorityGeneratorFabricator<NestedGenerator, tests::BGen>();

    EXPECT_EQ(gen.generate(node.get()), "word 5 word");
}

TEST(PriorityGenIntegrationTest, GeneratePutFront) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    auto gen = cg::PriorityGenerator();

    gen.put_front(std::make_unique<tests::AGen>());
    gen.put_front(std::make_unique<tests::BGen>());

    EXPECT_EQ(gen.generate(node.get()), "word 5");
}

TEST(PriorityGenIntegrationTest, GeneratePutBack) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    auto gen = cg::PriorityGenerator();

    gen.put_back(std::make_unique<tests::AGen>());
    gen.put_back(std::make_unique<tests::BGen>());

    EXPECT_EQ(gen.generate(node.get()), "5 word");
}

TEST(NamedEntityGeneratorIntegrationTest, ReturnsEmptyWhenNoEntity) {
    cg::core::Node node;

    auto gen = cg::PriorityGenerator();

    EXPECT_EQ(gen.generate(&node), "");
}

TEST(NamedEntityGeneratorIntegrationTest, GenerateNameFromEntity) {
    std::string node_name = "word";

    auto ent = tests::EntityB(node_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityB>>(ent);

    auto gen = cg::PriorityGeneratorFabricator<tests::BGen>();

    EXPECT_EQ(gen.generate(node.get()), node_name);
}