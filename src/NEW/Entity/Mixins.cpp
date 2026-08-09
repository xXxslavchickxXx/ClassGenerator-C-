#pragma once

#include <Entity/Mixins.h>

#include <algorithm>
#include <iostream>
#include <Entity/AccessEntities.h>

namespace cg::source {
	void TemplateEntity::add_template(std::shared_ptr<TemplateArgument> arg) {
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

	const std::shared_ptr<Class> TemplateArgument::get_type() const {
		return type;
	}

	const std::vector<std::shared_ptr<Class>>& 
		TemplateArgument::get_instance() const {
		return instance_cls;
	}

	std::shared_ptr<TemplateArgument>
		TemplateEntity::get_template(const std::string& name) {
		auto it = std::find_if(templates.begin(), templates.end(),
			[&name](const auto& it) {
				return it->get_name() == name;
			}
		);

		if (it == templates.end()) throw std::runtime_error("template doesn't exist");
		return *it;
	}


	void TemplateArgument::instance(std::shared_ptr<Class> inst_cls) {
		if (type) reset_type();
		if (instance_cls.size() == 1 && !variadic)
			instance_cls.clear();
		instance_cls.push_back(std::move(inst_cls));
	}

	void TemplateArgument::reset_instance() {
		instance_cls.clear();
	}

	void TemplateArgument::reset_type() {
		type.reset();
	}
	void TemplateArgument::toggle_variadic() {
		variadic = !variadic;
	}
	bool TemplateArgument::is_variadic() const {
		return variadic;
	}
	void TemplateArgument::set_type(std::shared_ptr<Class> new_type) {
		if (instance_cls.size()) reset_instance();
		type = std::move(new_type);
	}

	bool TemplateArgument::is_template_argument() const {
		return template_argument;
	}
	void TemplateArgument::toggle_template_argument() {
		template_argument != template_argument;
		if (!is_typename()) reset_type();
	}

	TemplateArgument::TemplateArgument(const std::string& name)
	: NamedEntity(name), type(nullptr), variadic(false) {}

	bool TemplateArgument::is_typename() const {
		return type == nullptr;
	}

	const Access& AccessEntity::get_access() const {
		return access_;
	}
	void AccessEntity::set_access(const Access& access) {
		access_ = access;
	}

	std::vector<std::shared_ptr<TemplateArgument>>&
	TemplateEntity::get_templates() {
		return templates;
	}
	const std::vector<std::shared_ptr<TemplateArgument>>&
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