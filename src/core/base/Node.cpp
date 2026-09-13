#include <core/base/Node.h>

namespace cg::core {
    Node::Node() : Node(nullptr) {}
    Node::Node(Node* parent) : parent(parent), entities(std::make_unique<EntityHandler>()) {}

    Node::Node(Node&& other) : entities(std::move(other.entities)),
    parent(std::exchange(other.parent, nullptr)) {}
    
    Node& Node::operator=(Node&& other) {
        this->entities = std::move(other.entities);
        this->parent = std::exchange(other.parent, nullptr);
        return *this;
    }

    const Node* Node::get_parent() const {
        return parent;
    }
    
    EntityHandler* Node::get_entities() { return entities.get(); }
    const EntityHandler* Node::get_entities() const { return entities.get();}
    
    void Node::set_parent(Node* new_parent) {
        if (new_parent != this) parent = new_parent;
    }

    void TreeNode::add_child(std::unique_ptr<Node> child) {
        child->set_parent(this);
        children.push_back(std::move(child));
    }

    std::vector<std::unique_ptr<Node>>& TreeNode::get_children() { return children; }
    const std::vector<std::unique_ptr<Node>>& TreeNode::get_children() const { return children; }
}