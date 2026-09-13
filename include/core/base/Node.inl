namespace cg::core {
    template<typename... Entities>
    Node* TreeNode::create_child() {
        auto new_child = std::unique_ptr<Node>(this);
        ((new_child->get_entities()->add<Entities>()), ...);
        children.push_back(new_child);

        return new_child.get();
    }
    template<typename... Args>
    Node* TreeNode::create_child(Args... args) {
        auto new_child = std::unique_ptr<Node>(this);
        ((new_child->get_entities()->add<Args>(std::forward<Args>(args))), ...);
        children.push_back(new_child);

        return new_child.get();
    }
}