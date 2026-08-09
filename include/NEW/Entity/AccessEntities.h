#pragma once

#include <Entity/Node.h>
#include <Entity/Mixins.h>

namespace cg::source {

	class Class :
	public NodeEntity,
	public AccessEntity,
	public TemplateEntity,
	public DefinitionEntity,
	public FabricEntity<Class> {
		using Base = NodeEntity;

		bool can_be_child(const NodeType& type) override;
	public:
		Class(const std::string& name);

		using Base::add_child;
		using Base::swap_nodes;
		using Base::erase_child;
	};
}