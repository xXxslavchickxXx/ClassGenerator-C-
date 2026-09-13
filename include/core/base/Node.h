#pragma once

#include <memory>
#include <vector>

#include <core/base/EntityHandler.h>

namespace cg::core {
    class TreeNode;
    class INode {
    public:
        virtual ~INode() = default;
        virtual TreeNode* tree_cast() { return nullptr; }
    };

    class Node :
        public EntityHandler,
        public INode
    {
        std::weak_ptr<Node> parent;

    public:
        Node();
        Node(std::shared_ptr<Node> parent);

        const Node* get_parent() const;
        void set_parent(std::shared_ptr<Node> new_parent);
    };

    class TreeNode :
        public Node,
        public std::enable_shared_from_this<TreeNode>    
    {
        std::vector<std::shared_ptr<Node>> children;

    public:
        using Node::Node;

        TreeNode* tree_cast() override { return this; }
        
        template<typename... Entities>
        Node* create_child();
        template<typename... Args>
        Node* create_child(Args... args);

        void add_child(std::shared_ptr<Node> child);

        std::vector<std::shared_ptr<Node>>& get_children();
        const std::vector<std::shared_ptr<Node>>& get_children() const;
    };
}

#include "Node.inl"