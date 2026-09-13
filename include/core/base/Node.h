#pragma once

#include <memory>
#include <vector>

#include <core/base/EntityHandler.h>

namespace cg::core {
    class TreeNode;
    class INode {
    public:
        virtual ~INode() = default;
        virtual TreeNode* as_container() { return nullptr; }
    };

    class Node :
        public INode
    {
        Node* parent;
        std::unique_ptr<EntityHandler> entities;

    public:
        Node();
        Node(Node* parent);

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&& other);
        Node& operator=(Node&& other);

        EntityHandler* get_entities();
        const EntityHandler* get_entities() const;

        const Node* get_parent() const;
        void set_parent(Node* new_parent);
    };

    class TreeNode :
        public Node   
    {
        std::vector<std::unique_ptr<Node>> children;

    public:
        using Node::Node;

        TreeNode* as_container() override { return this; }
        
        template<typename... Entities>
        Node* create_child();
        template<typename... Args>
        Node* create_child(Args... args);

        void add_child(std::unique_ptr<Node> child);

        std::vector<std::unique_ptr<Node>>& get_children();
        const std::vector<std::unique_ptr<Node>>& get_children() const;
    };
}

#include "Node.inl"