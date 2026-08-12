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
		std::vector<std::shared_ptr<Class>> base_classes;

	public:
		using src::NamedEntity::NamedEntity;

		void add_base_class(src::Node* raw);
		void erase_base_class(size_t where);
		void swap_base_classes(size_t from, size_t to);

	};

	class Namespace
		: public src::CompositeNode<Namespace, Class, Variable, Function>,
		public src::NamedEntity
	{
	public:
		using src::NamedEntity::NamedEntity;
	};
}