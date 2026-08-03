#include <DefaultEntity/Node.h>

#include <algorithm>

namespace cg::source {

	void NodeEntity::add_children(std::shared_ptr<NodeEntity> child) {
        child->parent_ = shared_from_this();
        children_.push_back(std::move(child));
    }

    void NodeEntity::swap_nodes(size_t from, size_t to) {
        std::iter_swap(children_.begin() + from, children_.begin() + to);
    }

    NodeType NodeEntity::get_node_type() const { return type_; }
}