#pragma once

#include <Node/Node.h>
#include <Entity/Mixin.h>
#include <vector>

namespace cg::ent {
	class Variable
		: public src::Node,
		public src::NamedEntity,
		public src::DefinitionEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Function
		: public src::Node,
		public src::NamedEntity,
		public src::DefinitionEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Field
		: public src::Node,
		public src::NamedEntity,
		public src::DefinitionEntity,
		public src::AccessEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Method
		: public src::Node,
		public src::NamedEntity,
		public src::DefinitionEntity,
		public src::AccessEntity
	{
	public:
		using src::NamedEntity::NamedEntity;

	};

	class Class
		: public src::CompositeNode<Class, Field, Method>,
		public src::NamedEntity,
		public src::AccessEntity
	{
		//std::vector<std::pair<std::shared_ptr<Class>, src::Access>> base_classes;

	public:
		using src::NamedEntity::NamedEntity;

	};

	class Namespace
		: public src::CompositeNode<Namespace, Class, Variable, Function>,
		public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;
	};
}