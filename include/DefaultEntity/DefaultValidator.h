#pragma once

#include <ErrorEvents.h>
#include <DefaultEntity/DefaultEntity.h>
#include <DefaultEntity/DefaultGenerator.h>
#include <iostream>
#include <concepts>

namespace cg::validate {
/// Объявления
	template<typename T = source::TypeName>
	concept TemplateNamedEntity = (std::is_base_of_v<source::TemplateEntity, T> &&
		std::is_base_of_v<source::NamedEntity, T>);

	// Валидаторы
	class Validator {
	protected:
		pl::error::ConflictResolver<CGError, CGContext> cr;

		// @brief:
		// Я хз как и к чему это привязать, так как
		// эта валидация нужна и алиасам и методам с полями,
		// слишком сложно абстрагироваться, лучше так
		//
		// @args:
		// required_list - список Типов, которые могут быть шаблонными, иначе исключение
		// 
		// local - это ближайшая сущность в которой обязан быть шаблон из
		// зарезервированного_списка
		//
		// external - аргумент на случай если есть внешняя сущность
		// вроде класса или структуры. Опциональный аргумент
		void template_bind_validate(std::vector<source::TypeName>& required_list,
			source::TemplateEntity& local,
			source::TemplateEntity& external);
		
		void template_bind_validate(std::vector<source::TypeName>& required_list,
			source::TemplateEntity& local);

		void recursive_template_collector(const source::TypeName& type, std::vector<source::TypeName>& accumulator);

	public:
		virtual ~Validator() = default;
		Validator();

		pl::error::ConflictResolver<CGError, CGContext>& get_conflict_resolver();
	};

	// @brief:
	// Валидатор проверяет только сущности обладающие шаблонным списком,
	// который может использоваться под генерацию деклараций.
	// Это потому что, ему надо чтобы шаблон был референсом чего-то
	// то есть иметь тайпнейм
	class TemplateParamValidator : public Validator {
	public:
		void verify(source::TemplateEntity& entity);
	};

	class FunctionValidator : public Validator {
		TemplateParamValidator temp_valid;

	public:
		void verify(source::Function& func);

	};

	class VariableValidator : public Validator {
	public:
		void verify(source::Variable& var);
	};
}

namespace cg::validate {
/// Реализации
	inline void FunctionValidator::verify(source::Function& func) {
		std::vector<source::TypeName> req_list;

		for (const auto& ns : func.get_namespace()) { req_list.push_back(ns.name); }
		for (const auto& arg : func.get_args()) { req_list.push_back(arg.get_type()); }
		req_list.push_back(func.get_type());

		temp_valid.verify(func);

		template_bind_validate(req_list, func);
	}

	inline void VariableValidator::verify(source::Variable& var) {
		if (var.get_type().is_template()) {
			CGContext ctx;
			ctx.type = CGError::TypeShouldNotBeTemplate;
			ctx.message = "entity: \"" +
				var.get_type().get_name()
				+ "\" " + "wouldn't be a template!";

			switch (cr.resolve(ctx)) {
			case pl::error::ResolutionStrategy::ApplyOptionA:
				var.get_type().toggle_templatebale();
				break;
				
			default:
				break;
			}
		}
	}

	inline void Validator::template_bind_validate(
		std::vector<source::TypeName>& required_list,
		source::TemplateEntity& local)
	{
		std::vector<source::TypeName> recursive_required_templates;
		for (const auto& req_type : required_list) {
			recursive_template_collector(req_type, recursive_required_templates);
		}

		const auto& declared_local_list = local.get_template_parametrs();

		for (const auto& require_template : recursive_required_templates) {
			if (!require_template.is_template()) continue;

			bool find = false;

			if (!find) {
				for (const auto& method_template : declared_local_list) {
					if (method_template.get_name() == require_template.get_name()) {
						find = true;
						break;
					}
				}
			}

			if (!find) {
				CGContext ctx;
				ctx.message = "entity: \"" + require_template.get_name() + "\" should be linked to entity!";

				ctx.type = CGError::TemplateTypeIsNotLinked_Local;

				switch (cr.resolve(ctx)) {
				case pl::error::ResolutionStrategy::ApplyOptionA:
					local.add_template_parametr(require_template);
					break;
				default:
					break;
				}
			}
		}
	}

	inline void Validator::template_bind_validate(
		std::vector<source::TypeName>& required_list,
		source::TemplateEntity& local,
		source::TemplateEntity& external)
	{
		std::vector<source::TypeName> recursive_required_templates;
		for (const auto& req_type : required_list) {
			recursive_template_collector(req_type, recursive_required_templates);
		}

		const auto& declared_local_list = local.get_template_parametrs();

		for (const auto& require_template : recursive_required_templates) {
			if (!require_template.is_template()) continue;

			bool find = false;

			for (const auto& class_template : external.get_template_parametrs()) {
				if (class_template.get_name() == require_template.get_name()) {
					find = true;
					break;
				}
			}
		
			if (!find) {
				for (const auto& method_template : declared_local_list) {
					if (method_template.get_name() == require_template.get_name()) {
						find = true;
						break;
					}
				}
			}

			if (!find) {
				CGContext ctx;
				ctx.message = "entity: \"" + require_template.get_name() + "\" should be linked to entity!";

				ctx.type = CGError::TemplateTypeIsNotLinked_External;

				switch (cr.resolve(ctx)) {
				case pl::error::ResolutionStrategy::ApplyOptionA:
					local.add_template_parametr(require_template);
					break;
				case pl::error::ResolutionStrategy::ApplyOptionB:
					external.add_template_parametr(require_template);
					break;
				default:
					break;
				}
			}
		}
	}

	inline void Validator::recursive_template_collector(
		const source::TypeName& type, std::vector<source::TypeName>& accumulator
	) {
		// Если сам тип является шаблонным параметром
		if (type.is_template()) {
			// Проверяем, нет ли его уже в аккумуляторе
			auto it = std::find_if(accumulator.begin(), accumulator.end(), [&](const source::TypeName& t) {
				return t.get_name() == type.get_name();
				});
			if (it == accumulator.end()) {
				accumulator.push_back(type);
			}
		}
		for (auto& t : type.get_namespace()) recursive_template_collector(t.name, accumulator);

		// А теперь идем вглубь по всем вложенным шаблонным аргументам
		// Например, для vec_type<Tag> мы зайдем в Tag
		// Для std::map<K, V> мы зайдем и в K, и в V
		for (const auto& nested_template_arg : type.get_template_parametrs()) {
			recursive_template_collector(nested_template_arg, accumulator);
		}
	}

	inline void TemplateParamValidator::verify(source::TemplateEntity& entity) {
		using namespace pl::error;

		for (auto& type : entity.get_template_parametrs()) {
			if (!type.is_template()) {
				CGContext ctx;
				ctx.type = CGError::TypeNotTemplate;
				ctx.message = "entity: \"" +
				type.get_name()
				+ "\" " + "would be template type!";

				switch(cr.resolve(ctx)) {
					case ResolutionStrategy::ApplyOptionA:
						std::cout << type.get_name() << " toggle to template type!\n";
						type.toggle_templatebale();
						break;
					default:
						break;
				};
			}
		}
	}

	inline pl::error::ConflictResolver<CGError, CGContext>& Validator::get_conflict_resolver() {
		return cr;
	}
}