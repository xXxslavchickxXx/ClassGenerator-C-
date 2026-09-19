#include <gtest/gtest.h>

#include <core/base/node/Node.h>

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
    auto another_tree_node = cg::core::TreeNode();
    
    auto* node_ptr = tree_node.create_child<cg::core::Node>();

    another_tree_node.add_child(tree_node.move_child(0));

    
    EXPECT_EQ(tree_node.children_size(), 0);
    EXPECT_EQ(another_tree_node.get_child(0), node_ptr);
}