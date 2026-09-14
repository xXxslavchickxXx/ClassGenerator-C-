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

    std::vector<std::unique_ptr<Node>>& TreeNode::get_children() { return children; }
    const std::vector<std::unique_ptr<Node>>& TreeNode::get_children() const { return children; }
}