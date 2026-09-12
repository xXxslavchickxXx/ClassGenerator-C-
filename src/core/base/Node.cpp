#include <core/base/Node.h>

namespace cg::core {
    Node::Node() : Node(nullptr) {}
    Node::Node(const Node* parent) : parent(parent) {}

    template<typename... Entities>
        requires (std::is_base_of_v<IEntity, std::remove_cvref_t<Entities>> && ...)
    Node::Node(const Node* p, Entities&&... entities) : parent(p) {
        (add(std::forward<Entities>(entities)), ...);
    }
    template<typename... Entities>
        requires (sizeof...(Entities) > 0) &&
        (std::is_base_of_v<IEntity, std::remove_cvref_t<Entities>> && ...)
    Node::Node(Entities&&... entities) : Node(nullptr, std::forward<Entities>(entities)...) {}

    const Node* Node::get_parent() const {
        return parent;
    }
    
    void Node::set_parent(const Node* new_parent) {
        parent = new_parent;
    }
}