namespace cg::core {
    template<typename NodeT, typename... Entities>
    NodeT* TreeNode::create_child() {
        static_assert(std::is_base_of_v<Node, NodeT>,
        "T must derive from Node");

        auto new_child = std::make_unique<NodeT>(this);
        auto* raw_ptr = new_child.get();
        ((new_child->get_entities().add<Entities>()), ...);
        children.push_back(std::move(new_child));

        return raw_ptr;
    }
    template<typename NodeT, typename... Entities>
    NodeT* TreeNode::create_child(Entities&&... ents) {
        static_assert(std::is_base_of_v<Node, NodeT>,
        "T must derive from Node");
        
        auto new_child = std::make_unique<NodeT>(this);
        auto* raw_ptr = new_child.get();
        ((new_child->get_entities().add<Entities>(std::forward<Entities>(ents))), ...);
        children.push_back(std::move(new_child));

        return raw_ptr;
    }
    template<typename T>
    T* TreeNode::add_child(std::unique_ptr<T> child) {
        static_assert(std::is_base_of_v<Node, T>,
        "T must derive from Node");
        
        if (child->set_parent(this)) {
            auto* raw_ptr = child.get();
            children.push_back(std::move(child));
            return raw_ptr;
        }
        return nullptr;
    }
}