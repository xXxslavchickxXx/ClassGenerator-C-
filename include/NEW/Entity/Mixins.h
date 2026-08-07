#pragma once

#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <memory>

namespace cg::source {	
	template<typename T>
	class FabricEntity {
	public:
		static std::shared_ptr<T> create(const std::string& name);
	};

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
	: public NamedEntity,
		public FabricEntity<TemplateArguement>
	{
		std::shared_ptr<Class> type;
		std::shared_ptr<Class> instance_cls;
		bool template_argument;
		bool variadic;

	public:
		TemplateArguement(const std::string& name);

		// Убирает инстанс неявно
		void set_type(std::shared_ptr<Class> new_type);

		// Инстанс убирает тайп неявно
		void instance(std::shared_ptr<Class> inst_cls);
		void reset_instance();

		bool is_template_argument() const;
		// Ресетает тип если такой был
		void toggle_template_argument();

		// тру если аргумент типовое имя, то есть typename
		bool is_typename() const;
		void reset_type();

		bool is_variadic() const;
		void toggle_variadic();

	};

	class TemplateEntity {
		std::vector<std::shared_ptr<TemplateArguement>> templates;

	public:
		void add_template(std::shared_ptr<TemplateArguement> arg);
		std::shared_ptr<TemplateArguement>
			get_template(const std::string& name);
		void erase_template(const std::string& name);

		std::vector<std::shared_ptr<TemplateArguement>>& get_templates();
		const std::vector<std::shared_ptr<TemplateArguement>>&
			get_templates() const;
		
		void swap_templates(size_t from, size_t to);

	private:
		bool have_variadic() const;

	};
}

#include <Entity/Mixins.inl>