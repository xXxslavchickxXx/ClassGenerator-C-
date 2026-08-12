#include <Node/Node.h>

namespace cg::src {

	const Node* Node::get_parent() const {
		return parent.lock().get();
	}
	Node* Node::get_parent() {
		return parent.lock().get();
	}

	void Node::set_parent(std::shared_ptr<Node> new_parent) {
		if (!new_parent) return;

		parent = std::move(new_parent);
	}

	void Node::reset_parent() {
		parent.reset();
	}
}