#pragma once

#include <Entity/Node.h>
#include <Entity/Mixins.h>

namespace cg::source {
	class Class :
	public NodeEntity,
	public AccessEntity,
	public TemplateEntity {
		using Base = NodeEntity;

	public:
		Class(const std::string& name);

		bool can_be_child(const NodeType& type) override;

		using Base::add_child;
		using Base::swap_nodes;
		using Base::erase_child;
	};
}