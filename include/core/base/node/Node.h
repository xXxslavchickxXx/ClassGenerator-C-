#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include <core/base/interfaces/INode.h>
#include <core/base/entity/EntityHandler.h>

namespace cg::core {
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
        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;

        EntityHandler* get_entities();
        const EntityHandler* get_entities() const;

        const Node* get_parent() const;
        bool set_parent(Node* new_parent);
    };

    class TreeNode :
        public Node   
    {
        std::vector<std::unique_ptr<Node>> children;

    public:
        TreeNode() = default;

        TreeNode* as_container() override { return this; }
        
        template<typename NodeT, typename... Entities>
        NodeT* create_child();
        template<typename NodeT, typename... Args>
        NodeT* create_child(Args&&... args);

        template<typename T>
        T* add_child(std::unique_ptr<T> child);

        std::vector<std::unique_ptr<Node>>& get_children();
        const std::vector<std::unique_ptr<Node>>& get_children() const;
    };
}

#include "Node.inl"