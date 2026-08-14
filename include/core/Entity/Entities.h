#pragma once

#include <Node/Node.h>
#include <Entity/Mixin.h>

#include <unordered_map>
#include <string>

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
		// TODO: заняться, когда сделаю шаблонность
		/*std::unordered_map<std::string,
						   std::pair<std::shared_ptr<Class>, src::Access>>
		base_classes;*/

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

	class Reference
		: public src::ITreeElement
	{
		std::shared_ptr<const Class> target;

	public:
		Reference() = default;
		Reference(const Class* cls);

		const ent::Class* get_target() const;
	};

	class Type
		: public src::Constantable,
		public src::InstanceList,
		public Reference
	{
		src::Qualificator qual;

	public:
		using Reference::Reference;

		Type(const Type&) = default;
		Type& operator=(const Type&) = default;
		Type(Type&&) = default;
		Type& operator=(Type&&) = default;

		src::Qualificator& get_type_qualificator();
		const src::Qualificator& get_type_qualificator() const;

	};

	class TemplateArgument
		: public Reference {

	};
}