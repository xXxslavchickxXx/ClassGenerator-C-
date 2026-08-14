#include <Entity/Mixin.h>

#include <Entity/Entities.h>
#include <algorithm>

namespace cg::src {
	NamedEntity::NamedEntity(const std::string& name) : name(name) {}

	const std::string& NamedEntity::get_name() const {
		return name;
	}

	void NamedEntity::set_name(const std::string& new_name) {
		name = new_name;
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

	bool Constantable::is_const() const {
		return constantable;
	}
	void Constantable::toggle_const() {
		constantable = !constantable;
	}

	bool Qualificator::has_qualificator() const {
		return qual == QUALIFICATOR::NON_QUAL;
	}
	const QUALIFICATOR& Qualificator::get_qualificator() const {
		return qual;
	}

	void Qualificator::set_qualificator(const QUALIFICATOR& new_qual) {
		qual = new_qual;
	}
	void Qualificator::toggle_to_pointer() {
		qual = QUALIFICATOR::POINTER;
	}
	void Qualificator::toggle_to_reference() {
		qual = QUALIFICATOR::REFERENCE;
	}
	void Qualificator::toggle_to_uni_ref() {
		qual = QUALIFICATOR::UNIVERSAL_REFERENCE;
	}
	void Qualificator::reset_qualificator() {
		qual = QUALIFICATOR::NON_QUAL;
		if (is_const()) toggle_const();
	}

	const std::vector<ent::Type>
		InstanceList::get_instance_list() const {
		return instance_list;
	}
	std::vector<ent::Type>
		InstanceList::get_instance_list() {
		return instance_list;
	}

	void InstanceList::add_instance(const ent::Type& type) {
		instance_list.push_back(type);
	}
	size_t InstanceList::instance_count() const {
		return instance_list.size();
	}
	void InstanceList::erase_instance(size_t where) {
		if (where >= instance_count()) return;

		instance_list.erase(instance_list.begin() + where);
	}

	void InstanceList::swap_instances(size_t from, size_t to) {
		if (from >= instance_count() ||
			to >= instance_count()) return;

		ent::Type temp = std::move(instance_list[from]);
		instance_list[from] = std::move(instance_list[to]);
		instance_list[to] = std::move(temp);
	}
}