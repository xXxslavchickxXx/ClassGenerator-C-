#include <algorithm>

namespace cg::src {
	template<typename T>
	T* ITreeElement::as() {
		return dynamic_cast<T*>(this);
	}

	template<typename T>
	const T* ITreeElement::as() const {
		return dynamic_cast<const T*>(this);
	}

	template<typename... SupportedT>
	template<typename T>
	void CompositeNode<SupportedT...>::add_child(std::shared_ptr<T> child) {
		static_assert(std::is_base_of_v<Node, T>,
			"Type would be base of Node");
		static_assert((std::is_same_v<SupportedT, T> || ...),
			"Node type is not allowed in this branch!");
		if (!child) return;

		child->set_parent(shared_from_this());
		children.push_back(std::move(child));
	}

	template<typename... SupportedT>
	const Node* CompositeNode<SupportedT...>::get_child(const size_t i) const {
		if (i >= size()) return nullptr;

		return children[i].get();
	}

	template<typename... SupportedT>
	size_t CompositeNode<SupportedT...>::size() const {
		return children.size();
	}

	template<typename... SupportedT>
	void CompositeNode<SupportedT...>::swap_child(size_t from, size_t to) {
		if (from >= size() || to >= size()) return;

		std::iter_swap(children.begin() + from, children.begin() + to);
	}

	template<typename... SupportedT>
	Node* CompositeNode<SupportedT...>::get_child(const size_t i) {
		if (i >= size()) return nullptr;

		return children[i].get();
	}

	template<typename... SupportedT>
	void CompositeNode<SupportedT...>::erase_child(size_t i) {
		if (i >= size()) return;

		children.erase(children.begin() + i);
	}
}