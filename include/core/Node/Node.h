#pragma once

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

	/// <summary>
	/// @brief класс ветвь, которая может иметь детей.
	/// 
	/// @details данный класс должен быть родителем классов, которые
	/// могут иметь детей, как например неймспейс или класс.
	/// Например методы или переменные не могут иметь детей.
	/// Класс также сам является нодой
	/// </summary>
	/// <typeparam name="...SupportedT">
	/// Данный вариативный шаблон говорит какие типы данная ветвь должна хранить,
	/// остальные под запретом и не будут добавляться по идее на этапе компиляции
	/// </typeparam>
	template<typename... SupportedT>
	class CompositeNode : public Node {
		std::vector<std::shared_ptr<Node>> children;

	public:
		using Node::Node;

		const Node* get_child(const size_t i) const;
		Node* get_child(const size_t i);

		template<typename T>
		void add_child(std::shared_ptr<T> child);
		void erase_child(size_t i);
		void swap_child(size_t from, size_t to);

		size_t size() const;

	private:
		constexpr bool is_not_allowed(const Node* node) {
			return (node->as<SupportedT>() || ...);
		}

	};
}

#include "Node.inl"