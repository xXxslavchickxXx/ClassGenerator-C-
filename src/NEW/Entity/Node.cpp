#include <Entity/Node.h>

#include <algorithm>
#include <iostream>

namespace cg::source {

    bool NodeEntity::dependent_node() const {
        return true;
    }

    NodeEntity::NodeEntity(NodeType type,
        const std::string& name)
        : type_(type),
        NamedEntity(name) {}

	void NodeEntity::add_child(std::shared_ptr<NodeEntity> child) {
        // this check checking can this node having this nodetype
        if (!can_be_child(child->get_node_type())) {
            std::cerr << "this node can't be child in this parent";
            return;
        }

        // Same names
        if (std::find_if(children_.begin(), children_.end(),
                [&child](const auto& it) {
                    if (((child->get_node_type() == 
                            NodeType::FUNCTION ||
                        child->get_node_type() ==
                            NodeType::METHOD)
                        
                        &&

                        child->get_node_type() ==
                            it->get_node_type())
                    )
                    return false;

                    return it->get_name() == child->get_name();
                }
            ) != children_.end())
        {
            std::cerr
            << "Entity with this name now exist in children list";
            return;
        }

        child->parent_ = shared_from_this();
        children_.push_back(std::move(child));
    }

    void NodeEntity::erase_child(const std::string& name) {
        auto it = std::find_if(children_.begin(), children_.end(),
            [&name](const auto& it) {
                return it->get_name() == name;
            }
        );
        
        if (it != children_.end()) children_.erase(it);
    }

    void NodeEntity::swap_nodes(size_t from, size_t to) {
        std::iter_swap(children_.begin() + from, children_.begin() + to);
    }

    NodeType NodeEntity::get_node_type() const { return type_; }
}