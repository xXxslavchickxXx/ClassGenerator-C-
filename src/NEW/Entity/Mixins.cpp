#pragma once

#include <Entity/Mixins.h>

#include <algorithm>
#include <iostream>
#include <Entity/AccessEntities.h>

namespace cg::source {
	void TemplateEntity::add_template(const TemplateArguement& arg) {
		auto it = std::find_if(templates.begin(), templates.end(),
			[&arg](const auto& it) {
				return it.get_name() == arg.get_name();
			}
		);

		if (it != templates.end()) {
			std::cerr
				<< "Entity with this name now exist in children list";
			return;
		}

		templates.push_back(arg);
	}
	void TemplateEntity::erase_template(const std::string& name) {
		auto it = std::find_if(templates.begin(), templates.end(),
			[&name](const auto& it) {
				return it.get_name() == name;
			}
		);

		if (it != templates.end()) templates.erase(it);
	}

	void TemplateArguement::reset_type() {
		type.reset();
	}
	void TemplateArguement::toggle_variadic() {
		variadic = !variadic;
	}
	bool TemplateArguement::is_variadic() const {
		return variadic;
	}
	void TemplateArguement::set_type(std::shared_ptr<Class> new_type) {
		type = std::move(new_type);
	}

	TemplateArguement::TemplateArguement(const std::string& name)
	: NamedEntity(name), type(nullptr), variadic(false) {}

	bool TemplateArguement::is_typename() const {
		return type == nullptr;
	}

	const Access& AccessEntity::get_access() const {
		return access_;
	}
	void AccessEntity::set_access(const Access& access) {
		access_ = access;
	}

	std::vector<TemplateArguement>&
	TemplateEntity::get_templates() {
		return templates;
	}
	const std::vector<TemplateArguement>&
	TemplateEntity::get_templates() const {
		return templates;
	}

	void TemplateEntity::swap_templates(size_t from, size_t to) {
		std::iter_swap(templates.begin() + from,
					   templates.begin() + to);
	}

	NamedEntity::NamedEntity(const std::string& name)
	: name_(name) {}

	void NamedEntity::set_name(const std::string& name) {
		name_ = name;
	}
	const std::string& NamedEntity::get_name() const {
		return name_;
	}
}