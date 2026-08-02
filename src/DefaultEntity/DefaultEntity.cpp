#include <AccessEntity/AccessEntity.h>

namespace cg::source {
	Variable::operator Arguement() const {
		auto arg = Arguement(name);
		arg.get_type() = get_type();
		arg.set_value(value);
		for (const auto& ns : namespace_prefix) {
			arg.add_namespace_prefix(ns);
		}
		return arg;
	}

	Namespace::Namespace(const std::string& name_)
		: name(name_) {}
	Namespace::Namespace(const char* name_)
		: name(std::string(name_)) {}
	Namespace::Namespace(const TypeName& t)
		: name(t) {}

	TypeName& Namespace::get_name() {
		return name;
	}
	const TypeName& Namespace::get_name() const {
		return name;
	}
	void Namespace::add_variable(const Variable& v) {
		entities.push_back(v);
	}
	void Namespace::add_function(const Function& f) {
		entities.push_back(f);
	}
	void Namespace::add_alias(const Alias& a) {
		entities.push_back(a);
	}
	void Namespace::add_class(const Class& class_) {
		entities.push_back(class_);
	}
	void Namespace::add_ns(const Namespace& ns) {
		entities.push_back(ns);
	}

	std::vector<namespace_entities>& Namespace::get_entities() {
		return entities;
	}
	const std::vector<namespace_entities>&
	Namespace::get_entities() const {
		return entities;
	}

	void Namespace::swap_entities(size_t i, size_t j) {
		if (i == j || i >= entities.size() ||
			j >= entities.size()) return;
		std::iter_swap(entities.begin() + i, entities.begin() + j);
	}
}