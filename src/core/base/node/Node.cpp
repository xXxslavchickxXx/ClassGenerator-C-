#include <core/base/node/Node.h>

#include <stdexcept>

namespace cg::core {
    Node::Node() : Node(nullptr) {}
    Node::Node(Node* parent) : parent(parent),entities(std::make_unique<EntityHandler>()) {}

    const Node* Node::get_parent() const {
        return parent;
    }
    
    EntityHandler* Node::get_entities() { return entities.get(); }
    const EntityHandler* Node::get_entities() const { return entities.get();}
    
    bool Node::set_parent(Node* new_parent) {
        if (new_parent != this) {
            parent = new_parent;
            return true;
        }
        return false;
    }

    const std::vector<std::unique_ptr<Node>>& TreeNode::get_children() const { return children; }

    Node* TreeNode::get_child(size_t where) {
        return children[where].get();
    }

    std::unique_ptr<Node> TreeNode::move_child(size_t where) {
        auto child = std::move(children[where]);
        child->set_parent(nullptr);

        children.erase(children.begin() + where);

        return child;
    }
    void TreeNode::erase_child(size_t where) {
        children.erase(children.begin() + where);
    }
}