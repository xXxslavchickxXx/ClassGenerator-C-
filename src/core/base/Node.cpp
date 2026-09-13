#include <core/base/Node.h>

namespace cg::core {
    Node::Node() : Node(nullptr) {}
    Node::Node(std::shared_ptr<Node> parent) : parent(parent) {}

    const Node* Node::get_parent() const {
        return parent.expired() ? parent.lock().get() : nullptr;
    }
    
    void Node::set_parent(std::shared_ptr<Node> new_parent) {
        parent = new_parent;
    }

    void TreeNode::add_child(std::shared_ptr<Node> child) {
        child->set_parent(shared_from_this());
        children.push_back(std::move(child));
    }

    std::vector<std::shared_ptr<Node>>& TreeNode::get_children() { return children; }
    const std::vector<std::shared_ptr<Node>>& TreeNode::get_children() const { return children; }
}