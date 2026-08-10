#pragma once

#include <Node/Node.h>
#include <Entity/Mixin.h>

namespace cg::ent {
	class Namespace
		: public src::CompositeNode, public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;
	};
}