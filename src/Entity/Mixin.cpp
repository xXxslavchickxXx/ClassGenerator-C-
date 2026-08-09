#include <Entity/Mixin.h>

namespace cg::src {
	NamedEntity::NamedEntity(const std::string& name) : name(name) {}

	const std::string& NamedEntity::get_name() const {
		return name;
	}

	void NamedEntity::set_name(const std::string& new_name) {
		name = new_name;
	}
}