#include <Node/Node.h>

#include <algorithm>

namespace cg::src {

	const Node* Node::get_parent() const {
		return parent.lock().get();
	}
	Node* Node::get_parent() {
		return parent.lock().get();
	}
	const Node* CompositeNode::get_child(const size_t i) const {
		if (i >= size()) return nullptr;

		return children[i].get();
	}

	size_t CompositeNode::size() const {
		return children.size();
	}
	void CompositeNode::swap(size_t from, size_t to) {
		if (from >= size() || to >= size()) return;

		std::iter_swap(children.begin() + from, children.begin() + to);
	}

	Node* CompositeNode::get_child(const size_t i) {
		if (i >= size()) return nullptr;

		return children[i].get();
	}

	void CompositeNode::erase_child(size_t i) {
		if (i >= size()) return;

		children.erase(children.begin() + i);
	}

	void Node::set_parent(std::shared_ptr<Node> new_parent) {
		if (!new_parent) return;

		parent = std::move(new_parent);
	}

	void Node::reset_parent() {
		parent.reset();
	}

	void CompositeNode::add_child(std::shared_ptr<Node> child) {
		if (!child) return;

		child->set_parent(shared_from_this());
		children.push_back(std::move(child));
	}
}