#pragma once

#include <memory>
#include <vector>
#include <type_traits>

#include <core/base/serviceHandler/EntityHandler.h>

namespace cg::core {
    // FWD
    class TreeNode;

    class Node
    {
        Node* parent;
        
    protected:
        EntityHandler entities;

    public:
        Node();

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&& other) = delete;
        Node& operator=(Node&& other) = delete;

        EntityHandler& get_entities();
        const EntityHandler& get_entities() const;

        virtual TreeNode* as_container() { return nullptr; }
        virtual ~Node() = default;

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
        bool set_parent(Node* new_parent);
    };

    class TreeNode :
        public Node   
    {
        std::vector<std::unique_ptr<Node>> children;

    public:
        TreeNode();

        template<bool is_const>
        class iterator;

    public:
        TreeNode* as_container() override { return this; }
        
        template<typename NodeT, typename... Entities>
        NodeT* create_child();
        template<typename NodeT, typename... Entities>
        NodeT* create_child(Entities&&... ents);

        /// @brief Добавляет ребёнка в дерево.
        /// @param child rvalue-ссылка на unique_ptr.
        /// @return Указатель на добавленного ребёнка, или nullptr при неудаче.
        /// @warning При успехе владение *переходит* в дерево.
        ///          При неудаче владение *остаётся* у вызывающего.
        template<typename T>
        T* add_child(std::unique_ptr<T>&& child);

        const std::vector<std::unique_ptr<Node>>& get_children() const;
        Node* get_child(size_t where);

        std::unique_ptr<Node> move_child(size_t where);
        void erase_child(size_t where);

        size_t children_size() const;

        iterator<false> begin() { return {children.data()}; }
        iterator<false> end() { return {children.data() + children.size()}; }

        iterator<true> begin() const { return {children.data()}; }
        iterator<true> end() const { return {children.data() + children.size()}; }
    
    public:
        template<bool is_const>
        class iterator {
            using uptr = std::conditional_t<is_const, const std::unique_ptr<Node>*, std::unique_ptr<Node>*>;
            using rptr = std::conditional_t<is_const, const Node*, Node*>;

            uptr p;
        public:
            iterator(uptr data) : p(data) {}

            rptr operator*() const { return p->get(); }
            rptr operator->() const { return p->get(); }
            iterator<is_const>& operator++() { ++p; return *this; }
            iterator<is_const>& operator--() { --p; return *this; }
            iterator<is_const> operator++(int) { auto temp = *this; ++p; return temp; }
            iterator<is_const> operator--(int) { auto temp = *this; --p; return temp; }
            iterator<is_const> operator+(const size_t num) const { auto temp = *this; temp += num; return temp; }
            iterator<is_const>& operator+=(const size_t num) { p += num; return *this; }
            iterator<is_const> operator-(const size_t num) const { auto temp = *this; temp -= num; return temp; }
            iterator<is_const>& operator-=(const size_t num) { p -= num; return *this; }
            bool operator!=(const iterator<is_const>& o) const { return p != o.p; }
            bool operator==(const iterator<is_const>& o) const { return p == o.p; }

        };
    };
}

#include "Node.inl"