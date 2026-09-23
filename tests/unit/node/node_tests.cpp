#include <gtest/gtest.h>

#include <core/base/node/Node.h>
#include <string>

namespace tests {
    struct EntityA : public cg::core::IEntity {
        int value = 0;

        EntityA(int val = 0) : value(val) {}

        ~EntityA() = default;
    };
    
    struct EntityB : public cg::core::IEntity {
        std::string word;
    
        EntityB(const std::string& word_ = "") : word(word_) {}
        ~EntityB() = default;
    };
}

TEST(Node, DefaultCtorHasNullParent) {
    auto node = cg::core::Node();

    EXPECT_EQ(node.get_parent(), nullptr);
}

TEST(Node, CtorWithParentStoresParent) {
    auto parent = cg::core::Node();
    auto son = cg::core::Node(&parent);

    EXPECT_EQ(son.get_parent(), &parent);
}

TEST(TreeNode, CreateChildReturnsNodeWithCorrectParent) {
    auto tree_node = cg::core::TreeNode();
    auto* node = tree_node.create_child<cg::core::Node>();

    EXPECT_EQ(node->get_parent(), &tree_node);
}

TEST(TreeNode, CreateIteratorAndMatchNode) {
    auto tree_node = cg::core::TreeNode();
    auto* first_node = tree_node.create_child<cg::core::Node>();
    tree_node.create_child<cg::core::Node>();
    auto* last_node = tree_node.create_child<cg::core::Node>();

    EXPECT_EQ(*tree_node.begin(), first_node);
    EXPECT_EQ(*(tree_node.end() - 1), last_node);
}

TEST(TreeNode, ForEachIteration) {
    auto tree_node = cg::core::TreeNode();

    int child_counter = 4;
    for (size_t i = 0; i < child_counter; i++)
        tree_node.create_child<cg::core::Node>();

    int test_counter = 0;
    for (auto* node : tree_node) { node; test_counter++; }

    EXPECT_EQ(child_counter, test_counter);
}

TEST(TreeNode, MoveChild) {
    auto tree_node = cg::core::TreeNode();
    
    tree_node.create_child<cg::core::Node>();

    auto moved_child = tree_node.move_child(0);

    EXPECT_EQ(tree_node.children_size(), 0);
    EXPECT_EQ(moved_child->get_parent(), nullptr);
}

TEST(TreeNode, AddChild) {
    auto tree_node = cg::core::TreeNode();
    
    auto child = std::make_unique<cg::core::Node>();

    auto* moved_child = tree_node.add_child(std::move(child));
    ASSERT_NE(moved_child, nullptr);

    EXPECT_EQ(tree_node.get_child(0), moved_child);
}

TEST(TreeNode, MoveChildAndTake) {
    auto tree_node = cg::core::TreeNode();
    auto another_tree_node = cg::core::TreeNode();
    
    auto* node_ptr = tree_node.create_child<cg::core::Node>();

    auto* moved_child = another_tree_node.add_child(tree_node.move_child(0));
    ASSERT_NE(moved_child, nullptr);

    EXPECT_EQ(another_tree_node.get_child(0), node_ptr);
    EXPECT_EQ(another_tree_node.get_child(0)->get_parent(), &another_tree_node);
}

TEST(TreeNode, EraseChild) {
    auto tree_node = cg::core::TreeNode();
    tree_node.create_child<cg::core::Node>();

    tree_node.erase_child(0);

    EXPECT_EQ(tree_node.children_size(), 0);
}

TEST(TreeNode, FailedAddChildWithoutLosses) {
    auto dummy_parent = cg::core::TreeNode();

    // Let him have some kind of parent to check for
    // this specific case involving data preservation.
    auto some_child = std::make_unique<cg::core::TreeNode>(&dummy_parent);

    // Trying add child to yourself
    auto* move_result = some_child->add_child(std::move(some_child));
    EXPECT_TRUE(some_child != nullptr);
    EXPECT_EQ(move_result, nullptr);

    // If the data could not be moved, the parent element
    // will remain as it was.
    EXPECT_EQ(some_child->get_parent(), &dummy_parent);
}

TEST(TreeNode, CreateChildWithoutData) {
    auto some_node = cg::core::TreeNode();

    auto* child = some_node.create_child<cg::core::Node, tests::EntityA>();
    auto& ents = child->get_entities();
    
    EXPECT_TRUE(ents.has<tests::EntityA>());
    EXPECT_FALSE(ents.has<tests::EntityB>());
}

TEST(TreeNode, CreateChildWithData) {
    auto some_node = cg::core::TreeNode();

    int test_value = 6;
    auto some_serv = tests::EntityA(test_value);

    auto* child = some_node.create_child<cg::core::Node>(some_serv);
    auto& ents = child->get_entities();
    ASSERT_NE(ents.get<tests::EntityA>(), nullptr);
    
    EXPECT_EQ(ents.get<tests::EntityA>()->value, test_value);
}