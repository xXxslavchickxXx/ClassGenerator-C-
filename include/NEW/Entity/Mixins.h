#pragma once

#include <vector>
#include <string>
#include <variant>
#include <optional>
#include <memory>

namespace cg::source {
	/// <summary>
	/// @brief По сути генерацию определения можно
	/// выделить как отдельный флаг, так как определения
	/// может вовсе не быть или быть, но под конкретный запрос
	/// под платформу или кроссплатформенный код
	/// </summary>
	enum class DEFINITION_TYPE {
		NO_DEFINITION = 0,
		CROSSPLATOFORM = 1 << 0,
		WINDOWS = 1 << 1,
		LINUX = 1 << 2,
		APPLE = 1 << 3
	};

	constexpr DEFINITION_TYPE operator|(DEFINITION_TYPE a, DEFINITION_TYPE b) {
		return static_cast<DEFINITION_TYPE>(
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(a) |
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(b)
			);
	}

	constexpr DEFINITION_TYPE& operator|=(DEFINITION_TYPE& a, DEFINITION_TYPE b) {
		a = static_cast<DEFINITION_TYPE>(
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(a) |
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(b)
			);
		return a;
	}

	constexpr bool has_platform(DEFINITION_TYPE mask, DEFINITION_TYPE check) {
		return (static_cast<std::underlying_type_t<DEFINITION_TYPE>>(mask) &
			static_cast<std::underlying_type_t<DEFINITION_TYPE>>(check)) != 0;
	}

	class DefinitionEntity {
		DEFINITION_TYPE type;

	public:
		DefinitionEntity(
			const DEFINITION_TYPE& definition =
			DEFINITION_TYPE::NO_DEFINITION)
			: type(definition) {}

		const DEFINITION_TYPE& get_definition() const { return type; }
		void set_definition(const DEFINITION_TYPE& definition) {
			type = definition;
		}

		bool has_definition() const {
			return type != DEFINITION_TYPE::NO_DEFINITION;
		}
	};

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

	class TemplateArgument
	: public NamedEntity,
		public FabricEntity<TemplateArgument>
	{
		std::shared_ptr<Class> type;
		std::vector<std::shared_ptr<Class>> instance_cls;
		bool template_argument;
		bool variadic;

	public:
		TemplateArgument(const std::string& name);

		// Убирает инстанс неявно
		void set_type(std::shared_ptr<Class> new_type);
		const std::shared_ptr<Class> get_type() const;

		// Инстанс убирает тайп неявно
		void instance(std::shared_ptr<Class> inst_cls);
		const std::vector<std::shared_ptr<Class>>& get_instance() const;
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
		std::vector<std::shared_ptr<TemplateArgument>> templates;

	public:
		void add_template(std::shared_ptr<TemplateArgument> arg);
		std::shared_ptr<TemplateArgument>
			get_template(const std::string& name);
		void erase_template(const std::string& name);

		std::vector<std::shared_ptr<TemplateArgument>>& get_templates();
		const std::vector<std::shared_ptr<TemplateArgument>>&
			get_templates() const;
		
		void swap_templates(size_t from, size_t to);

	private:
		bool have_variadic() const;

	};
}

#include <Entity/Mixins.inl>