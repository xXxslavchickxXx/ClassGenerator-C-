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
TEST(Node, SetParentIgnoresSelf) {
    auto node = cg::core::Node();
    node.set_parent(&node);

    EXPECT_EQ(node.get_parent(), nullptr);
}
TEST(TreeNode, AddChildSetsParentOfChild) {
    auto tree_node = cg::core::TreeNode();
    auto* node = tree_node.add_child(std::make_unique<cg::core::Node>());
    tree_node.set_parent(node);

    EXPECT_EQ(node->get_parent(), &tree_node);
    EXPECT_EQ(node->get_parent()->get_parent(), node);
}
TEST(TreeNode, CreateChildReturnsNodeWithCorrectParent) {
    auto tree_node = cg::core::TreeNode();
    auto* node = tree_node.create_child<cg::core::Node>();

    EXPECT_EQ(node->get_parent(), &tree_node);
}