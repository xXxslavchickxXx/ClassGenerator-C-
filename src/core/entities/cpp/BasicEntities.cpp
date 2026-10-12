#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>
#include <core/entities/cpp/EntitiyMarkers.h>

namespace cg::entities::cpp {
	ArgumentList::ArgumentList(std::initializer_list<const cg::core::Node*> arg_list) {
        for (auto* el : arg_list)
            if (!el->get_entities().has<cg::entities::cpp::IArgument>())
                throw std::runtime_error("all elements of arg list would derived from IArgument");
            
        for (auto* el : arg_list)
            arguments.push_back(el);
    }

	ArgumentList& ArgumentList::push_back(const cg::core::Node* node) {
		if (!node->get_entities().has<cg::entities::cpp::IArgument>())
                throw std::runtime_error("all elements of arg list would derived from IArgument");
		
		arguments.push_back(node);

		return *this;
	}
	void ArgumentList::erase(it iter) {
		arguments.erase(iter);
	}
	void ArgumentList::erase(size_t iter) {
		arguments.erase(arguments.begin() + iter);
	}

	ArgumentList::it ArgumentList::begin() {
		return arguments.begin();
	}
	ArgumentList::it ArgumentList::end() {
		return arguments.end();
	}
	
	ArgumentList::const_it ArgumentList::begin() const {
		return arguments.begin();
	}
	ArgumentList::const_it ArgumentList::end() const {
		return arguments.end();
	}

	TypeHandler::TypeHandler(const cg::core::TreeNodeBase* type) : type(type) {}

	const cg::core::TreeNodeBase* TypeHandler::get_type() const {
		return type;
	}
	void TypeHandler::set_type(const cg::core::TreeNodeBase* new_type) {
		if (
			!new_type->get_entities().has<IClass>()
				&&
			!new_type->get_entities().has<IAlias>()
		) return;

		type = new_type;
	}

	ClassHandler::ClassHandler(const cg::core::Node* nested_class) : nested_class(nested_class) {}

	const cg::core::Node* ClassHandler::get_class() const {
		return nested_class;
	}
	void ClassHandler::set_class(const cg::core::Node* new_ref) {
		if (
			!new_ref->get_entities().has<IClass>()
				&&
			!new_ref->get_entities().has<IAlias>()
		) return;

		nested_class = new_ref;
	}

	NamedEntity::NamedEntity(const std::string& new_name) : name(new_name) {}
	void NamedEntity::set_name(const std::string& new_name) { name = new_name; }
	const std::string& NamedEntity::get_name() const { return name; }

	DefinitionEntity::DefinitionEntity(
		const DEFINITION_TYPE& definition)
		: type(definition) {
	}

	const DEFINITION_TYPE& DefinitionEntity::get_definition() const { return type; }
	DefinitionEntity& DefinitionEntity::set_definition(const DEFINITION_TYPE& definition) {
		type = definition;
		return *this;
	}

	bool DefinitionEntity::has_definition() const {
		return type != DEFINITION_TYPE::NO_DEFINITION;
	}

	const Access& AccessEntity::get_access() const {
		return access_;
	}
	AccessEntity& AccessEntity::set_access(const Access& access) {
		access_ = access;
		return *this;
	}

	AccessEntity::AccessEntity(Access access) :
	access_(access) {}

	TypeQualificator::TypeQualificator() : mask(TYPE_QUAL::NONE) {}
	bool TypeQualificator::is_const() const { return (mask & TYPE_QUAL::CONSTANT) == TYPE_QUAL::CONSTANT; }
	TypeQualificator& TypeQualificator::toggle_const() { mask ^= TYPE_QUAL::CONSTANT; return *this; }
	bool TypeQualificator::is_volatile() const { return (mask & TYPE_QUAL::VOLATILE) == TYPE_QUAL::VOLATILE; }
	TypeQualificator& TypeQualificator::toggle_volatile() { mask ^= TYPE_QUAL::VOLATILE; return *this; }
	TypeQualificator& TypeQualificator::set_mask(const TYPE_QUAL& _mask) { mask = _mask; return *this; }


	TypeConstructor::TypeConstructor() : type(TYPE_CONSTRUCTORS::NON_QUAL) {}
	bool TypeConstructor::has_qualificator() const { return type != TYPE_CONSTRUCTORS::NON_QUAL; }
	const TYPE_CONSTRUCTORS& TypeConstructor::get_qualificator() const { return type; }
	TypeConstructor& TypeConstructor::set_qualificator(const TYPE_CONSTRUCTORS& new_qual) { type = new_qual; return *this; }
	TypeConstructor& TypeConstructor::toggle_to_pointer() { type = TYPE_CONSTRUCTORS::POINTER; return *this; }
	TypeConstructor& TypeConstructor::toggle_to_reference() { type = TYPE_CONSTRUCTORS::REFERENCE; return *this; }
	TypeConstructor& TypeConstructor::toggle_to_uni_ref() { type = TYPE_CONSTRUCTORS::UNIVERSAL_REFERENCE; return *this; }
	TypeConstructor& TypeConstructor::reset_qualificator() { type = TYPE_CONSTRUCTORS::NON_QUAL; return *this; }
	
	TypeConstructor& TypeConstructor::toggle_const() {
		TypeQualificator::toggle_const();
		return *this;
	}
	TypeConstructor& TypeConstructor::toggle_volatile() {
		TypeQualificator::toggle_volatile();
		return *this;
	}
}