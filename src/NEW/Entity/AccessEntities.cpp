#pragma once

#include <Entity/AccessEntities.h>

namespace cg::source {
	Class::Class(const std::string& name)
	: NodeEntity(NodeType::CLASS, name) {}

	bool Class::can_be_child(const NodeType& type) {
		return
		 (type != NodeType::FUNCTION) &&
		 (type != NodeType::VARIABLE) &&
		 (type != NodeType::NAMESPACE);
	}
}