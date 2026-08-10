#pragma once

#include <vector>
#include <memory>
#include <Entity/Mixins.h>

namespace cg::source {
    enum class NodeType {
        CLASS,
        FUNCTION,
        METHOD,
        VARIABLE,
        ALIAS,
        NAMESPACE,
        FIELD
    };

    class NodeEntity
    : public std::enable_shared_from_this<NodeEntity>,
    public NamedEntity {
    protected:
        NodeType type_;

        std::shared_ptr<NodeEntity> parent_;
        std::vector<std::shared_ptr<NodeEntity>> children_;

        NodeEntity(NodeType type,
            const std::string& name);

    public:
        NodeEntity(const NodeEntity&) = default;
        NodeEntity& operator=(const NodeEntity&) = default;
        NodeEntity(NodeEntity&&) = default;
        NodeEntity& operator=(NodeEntity&&) = default;

        virtual ~NodeEntity() = default;

        NodeType get_node_type() const;

        bool dependent_node() const;

    protected:
        virtual bool can_be_child(const NodeType& type) = 0;
        void add_child(std::shared_ptr<NodeEntity> child);
        void erase_child(const std::string& name);
        void swap_nodes(size_t from, size_t to);
    };
}