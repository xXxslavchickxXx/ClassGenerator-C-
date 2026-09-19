#include <core/entities/cpp/basic_entites.h>

namespace cg::entities::cpp {
	NamedEntity::NamedEntity(const std::string& new_name) : name(new_name) {}
	void NamedEntity::set_name(const std::string& new_name) { name = new_name; }
	const std::string& NamedEntity::get_name() const { return name; }

	DefinitionEntity::DefinitionEntity(
		const DEFINITION_TYPE& definition)
		: type(definition) {
	}

	const DEFINITION_TYPE& DefinitionEntity::get_definition() const { return type; }
	void DefinitionEntity::set_definition(const DEFINITION_TYPE& definition) {
		type = definition;
	}

	bool DefinitionEntity::has_definition() const {
		return type != DEFINITION_TYPE::NO_DEFINITION;
	}

	TypeQualificator::TypeQualificator() : mask(TYPE_QUAL::NONE) {}
	bool TypeQualificator::is_const() const { return (mask & TYPE_QUAL::CONSTANT) == TYPE_QUAL::CONSTANT; }
	void TypeQualificator::toggle_const() { mask ^= TYPE_QUAL::CONSTANT; }
	bool TypeQualificator::is_volatile() const { return (mask & TYPE_QUAL::VOLATILE) == TYPE_QUAL::VOLATILE; }
	void TypeQualificator::toggle_volatile() { mask ^= TYPE_QUAL::VOLATILE; }
	void TypeQualificator::set_mask(const TYPE_QUAL& _mask) { mask = _mask; }


	TypeConstructor::TypeConstructor() : type(TYPE_CONSTRUCTORS::NON_QUAL) {}
	bool TypeConstructor::has_qualificator() const { return type != TYPE_CONSTRUCTORS::NON_QUAL; }
	const TYPE_CONSTRUCTORS& TypeConstructor::get_qualificator() const { return type; }
	void TypeConstructor::set_qualificator(const TYPE_CONSTRUCTORS& new_qual) { type = new_qual; }
	void TypeConstructor::toggle_to_pointer() { type = TYPE_CONSTRUCTORS::POINTER; }
	void TypeConstructor::toggle_to_reference() { type = TYPE_CONSTRUCTORS::REFERENCE; }
	void TypeConstructor::toggle_to_uni_ref() { type = TYPE_CONSTRUCTORS::UNIVERSAL_REFERENCE; }
	void TypeConstructor::reset_qualificator() { type = TYPE_CONSTRUCTORS::NON_QUAL; }
}