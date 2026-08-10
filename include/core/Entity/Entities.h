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

	class Class
		: public src::CompositeNode, public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Variable
		: public src::Node, public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Function
		: public src::Node, public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Field
		: public src::Node, public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Method
		: public src::Node, public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};
}