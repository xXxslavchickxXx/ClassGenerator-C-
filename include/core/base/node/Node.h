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
        
    protected:
        EntityHandler entities;

    public:
        Node();
        Node(Node* parent);

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;

        EntityHandler& get_entities();
        const EntityHandler& get_entities() const;

        const Node* get_parent() const;

        /// @brief Меняет родителя узла.
        ///
        /// @warning Не обновляет 'children' старого/нового родителя.
        ///          После вызова инвариант дерева может быть нарушен:
        ///          'old_parent->children всё ещё владеет узлом,
        ///          а 'new_parent->children' — нет.
        ///
        /// @note Безопасно, если:
        ///       - 'new_parent == nullptr' и узел не в 'children' ни у кого
        ///       - узел не в 'children' старого родителя
        ///
        /// @see TreeNode::reparent
        bool set_parent(Node* new_parent);
    };

    class TreeNode :
        public Node   
    {
        std::vector<std::unique_ptr<Node>> children;
        using const_iter = std::vector<std::unique_ptr<Node>>::const_iterator;

    public:
        TreeNode();
        TreeNode(Node* parent);

    public:
        class iterator {
            std::unique_ptr<Node>* p;
        public:
            iterator(std::unique_ptr<Node>* data) : p(data) {}

            Node* operator*() const { return p->get(); }
            Node* operator->() const { return p->get(); }
            iterator& operator++() { ++p; return *this; }
            iterator& operator--() { --p; return *this; }
            iterator operator+(const size_t num) { auto temp = *this; temp += num; return temp; }
            iterator& operator+=(const size_t num) { p += num; return *this; }
            iterator operator-(const size_t num) { auto temp = *this; temp -= num; return temp; }
            iterator& operator-=(const size_t num) { p -= num; return *this; }
            bool operator!=(const iterator& o) const { return p != o.p; }
            bool operator==(const iterator& o) const { return p == o.p; }

        };

        TreeNode* as_container() override { return this; }
        
        template<typename NodeT, typename... Entities>
        NodeT* create_child();
        template<typename NodeT, typename... Entities>
        NodeT* create_child(Entities&&... ents);

        /// @brief Добавляет ребёнка в дерево.
        /// @param child rvalue-ссылка на unique_ptr.
        /// @return Указатель на добавленного ребёнка, или nullptr при неудаче.
        /// @warning При успехе владение **переходит** в дерево (child становится пустым).
        ///          При неудаче владение **остаётся** у вызывающего.
        template<typename T>
        T* add_child(std::unique_ptr<T>&& child);

        const std::vector<std::unique_ptr<Node>>& get_children() const;
        Node* get_child(size_t where);

        std::unique_ptr<Node> move_child(size_t where);
        void erase_child(size_t where);

        size_t children_size() const;

        iterator begin() { return {children.data()}; }
        iterator end() { return {children.data() + children.size()}; }
        const_iter begin() const { return children.begin(); }
        const_iter end() const { return children.end(); }
    
    };
}

#include "Node.inl"