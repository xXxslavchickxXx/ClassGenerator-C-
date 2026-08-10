#pragma once

#include <memory>
#include <stdexcept>

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

namespace cg::verif {

    class ValidationError : public std::runtime_error {
    public:
        ValidationError(const std::string& msg) : std::runtime_error(msg) {}
    };

	void validate_namespace(const std::shared_ptr<ent::Namespace>& ns);
}