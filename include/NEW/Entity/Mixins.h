#pragma once

#include <vector>
#include <string>
#include <optional>
#include <memory>

namespace cg::source {
	class Class;

	enum class Access {
		PRIVATE,
		PROTECTED,
		PUBLIC
	};

	class NamedEntity {
		std::string name_;

	public:
		NamedEntity(const std::string& name);

		void set_name(const std::string& name);
		const std::string& get_name() const;
	};

	class AccessEntity {
		Access access_ = Access::PRIVATE;

	public:
		const Access& get_access() const;
		void set_access(const Access& access);
	};

	class TemplateArguement
	: public NamedEntity {
		std::shared_ptr<Class> type;
		bool variadic;

	public:
		TemplateArguement(const std::string& name);

		void set_type(std::shared_ptr<Class> new_type);

		bool is_typename() const;
		void reset_type();

		bool is_variadic() const;
		void toggle_variadic();

	};

	class TemplateEntity {
		std::vector<TemplateArguement> templates;

	public:
		void add_template(const TemplateArguement& arg);
		void erase_template(const std::string& name);

		std::vector<TemplateArguement>& get_templates();
		const std::vector<TemplateArguement>& get_templates() const;
		
		void swap_templates(size_t from, size_t to);
	};
}