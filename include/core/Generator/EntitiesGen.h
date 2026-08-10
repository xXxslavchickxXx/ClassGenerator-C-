#pragma once

#include <string>
#include <memory>

namespace cg {
	namespace ent {
		class Namespace;
		class Variable;
		class Function;
		class Method;
		class Field;
		class Class;
	}

	namespace src {
		class Node;
	}
}

namespace cg::gen {
	std::string generate(const src::Node* raw_node,
		bool declaration = true,
		const src::Node* ctx = nullptr);
	
	std::string generate(const ent::Namespace* ns,
			bool declration = true,
			const src::Node* ctx = nullptr);
}