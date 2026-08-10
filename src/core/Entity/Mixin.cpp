#include <Entity/Mixin.h>

namespace cg::src {
	NamedEntity::NamedEntity(const std::string& name) : name(name) {}

	const std::string& NamedEntity::get_name() const {
		return name;
	}

	void NamedEntity::set_name(const std::string& new_name) {
		name = new_name;
	}

	constexpr bool has_platform(DEFINITION_TYPE mask, DEFINITION_TYPE check) {
		return (static_cast<std::underlying_type_t<DEFINITION_TYPE>>(mask) &
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(check)) != 0;
	}

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
}