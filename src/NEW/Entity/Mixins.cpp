#pragma once

#include <Entity/Mixins.h>

#include <algorithm>
#include <iostream>
#include <Entity/AccessEntities.h>

namespace cg::source {
	void TemplateEntity::add_template(std::shared_ptr<TemplateArguement> arg) {
		auto it = std::find_if(templates.begin(), templates.end(),
			[&arg](const auto& it) {
				return it->get_name() == arg->get_name();
			}
		);

		if (it != templates.end()) {
			std::cerr
				<< "Entity with this name now exist in children list";
			return;
		}

		templates.push_back(std::move(arg));
	}
	void TemplateEntity::erase_template(const std::string& name) {
		auto it = std::find_if(templates.begin(), templates.end(),
			[&name](const auto& it) {
				return it->get_name() == name;
			}
		);

		if (it != templates.end()) templates.erase(it);
	}

	std::shared_ptr<TemplateArguement>
		TemplateEntity::get_template(const std::string& name) {
		auto it = std::find_if(templates.begin(), templates.end(),
			[&name](const auto& it) {
				return it->get_name() == name;
			}
		);

		if (it == templates.end()) throw std::runtime_error("template doesn't exist");
		return *it;
	}


	void TemplateArguement::instance(std::shared_ptr<Class> inst_cls) {
		if (type) reset_type();
		instance_cls = std::move(inst_cls);
	}

	void TemplateArguement::reset_instance() {
		instance_cls.reset();
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
		if (instance_cls) reset_instance();
		type = std::move(new_type);
	}

	bool TemplateArguement::is_template_argument() const {
		return template_argument;
	}
	void TemplateArguement::toggle_template_argument() {
		template_argument != template_argument;
		if (!is_typename()) reset_type();
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

	std::vector<std::shared_ptr<TemplateArguement>>&
	TemplateEntity::get_templates() {
		return templates;
	}
	const std::vector<std::shared_ptr<TemplateArguement>>&
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