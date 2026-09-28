#include <gtest/gtest.h>
#include <stubs.h>

TEST(EntityFabricator, DefaultCtorWithoutEntities) {
    auto node = TypeRegistryFabrica<cg::core::Node>();
    auto tree_node = TypeRegistryFabrica<cg::core::TreeNode>();

    SUCCEED();
}

TEST(EntityFabricator, DefaultCtorWithClearEntities) {
    auto node = TypeRegistryFabrica<cg::core::Node, tests::EntityA>();

    EXPECT_TRUE(node.get_entities().has<tests::EntityA>());
}

TEST(EntityFabricator, CtorWithObjEntity) {
    int test_num = 5;
    auto ent = tests::EntityA(test_num);

    auto node = TypeRegistryFabrica<cg::core::Node, tests::EntityA>(ent);
    ASSERT_TRUE(node.get_entities().has<tests::EntityA>());

    EXPECT_EQ(node.get_entities().get<tests::EntityA>()->value, test_num);
}

TEST(EntityFabricator, CtorWithObjEntities) {
    int test_num = 5;
    auto ent = tests::EntityA(test_num);

    auto node = TypeRegistryFabrica<cg::core::Node, tests::EntityA, tests::EntityB>(ent);
    ASSERT_TRUE(node.get_entities().has<tests::EntityA>());
    ASSERT_TRUE(node.get_entities().has<tests::EntityB>());

    EXPECT_EQ(node.get_entities().get<tests::EntityA>()->value, test_num);
}

TEST(EntityFabricator, CtorWithObjEntitiesUnorderedQuery) {
    int test_num = 5;
    auto ent_A = tests::EntityA(test_num);
    std::string test_word = "some";
    auto ent_B = tests::EntityB(test_word);

    auto node = TypeRegistryFabrica<cg::core::Node, tests::EntityA, tests::EntityB>(ent_B, ent_A);
    ASSERT_TRUE(node.get_entities().has<tests::EntityA>());
    ASSERT_TRUE(node.get_entities().has<tests::EntityB>());

    EXPECT_EQ(node.get_entities().get<tests::EntityA>()->value, test_num);
    EXPECT_EQ(node.get_entities().get<tests::EntityB>()->word, test_word);
}