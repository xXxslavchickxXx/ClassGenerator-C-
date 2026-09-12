#include <core/base/Node.h>

namespace cg::core {
    Node::Node() : Node(nullptr) {}
    Node::Node(const Node* parent) : parent(parent) {}

    const Node* Node::get_parent() const {
        return parent;
    }
    
    void Node::set_parent(const Node* new_parent) {
        parent = new_parent;
    }
}