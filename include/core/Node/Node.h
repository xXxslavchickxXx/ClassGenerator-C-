#include <memory>
#include <vector>

namespace cg::src {

	class Node : public std::enable_shared_from_this<Node> {
	protected:
		std::weak_ptr<Node> parent;

	public:
		Node(Node&&) = default;
		Node& operator=(Node&&) = default;
		Node() = default;
		virtual ~Node() = default;

		void set_parent(std::shared_ptr<Node> new_parent);
		void reset_parent();

		const Node* get_parent() const;
		Node* get_parent();

		template<typename T>
		T* as();

		template<typename T>
		const T* as() const;

	};

	class CompositeNode : public Node {
		std::vector<std::shared_ptr<Node>> children;

	public:
		using Node::Node;

		const Node* get_child(const size_t i) const;
		Node* get_child(const size_t i);

		void add_child(std::shared_ptr<Node> child);
		void erase_child(size_t i);
		void swap(size_t from, size_t to);

		size_t size() const;
	};
}

#include "Node.inl"