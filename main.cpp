#include <iostream>

#include <Node/Node.h>

using namespace cg::src;

struct B {
	void some_mesag() {
		std::cout << "im here";
	}
};

struct A : public Node, public B {};

int main() {

	auto root_node = std::make_shared<CompositeNode>();
	auto root_node_child = std::make_shared<CompositeNode>();
	auto child = std::make_shared<Node>();
	auto child_side_class = std::make_shared<A>();

	root_node->add_child(root_node_child);
	root_node->add_child(child);
	root_node->add_child(child_side_class);

	if (root_node->get_child(2)->as<B>())
		std::cout << "hello template project!";
}