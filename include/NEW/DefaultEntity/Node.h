#pragma once

#include <vector>
#include <memory>

namespace cg::source {
    enum class NodeType {
        CLASS
    };

    class NodeEntity
    : public std::enable_shared_from_this<NodeEntity> {
    protected:
        NodeType type_;
        std::shared_ptr<NodeEntity> parent_;
        std::vector<std::shared_ptr<NodeEntity>> children_;

        NodeEntity(NodeType type) : type_(type) {}

    public:
        NodeEntity(const NodeEntity&) = default;
        NodeEntity& operator=(const NodeEntity&) = default;
        NodeEntity(NodeEntity&&) = default;
        NodeEntity& operator=(NodeEntity&&) = default;

        virtual ~NodeEntity() = default;

        void add_children(std::shared_ptr<NodeEntity> child);
        void swap_nodes(size_t from, size_t to);
        NodeType get_node_type() const;
    };
}