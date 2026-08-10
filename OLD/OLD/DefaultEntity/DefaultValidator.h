#pragma once

#include <ErrorEvents.h>
#include <AccessEntity/AccessEntity.h>
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
		bool template_bind_validate(std::vector<source::TypeName>& required_list,
			source::TemplateEntity& local,
			source::TemplateEntity& external);
		
		bool template_bind_validate(std::vector<source::TypeName>& required_list,
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

	class ArguementValidator : public Validator {
	public:
		void verify(source::ArgumentableEntity& args);
	};
}

namespace cg::validate {
/// Реализации
	inline void ArguementValidator::verify(source::ArgumentableEntity& args) {
		auto& list = args.get_args();

		int variadic_type = -1;

		for (size_t i = 0; i < list.size(); i++) {
			if (list[i].get_type().is_variadic()) {
				if (variadic_type == -1) {
					variadic_type = i;
				}
				else {
					CGContext ctx;
					ctx.type = CGError::MuchVariadicTemplates;
					ctx.message = "entity: \"" +
						list[i].get_name() +
						"\" and \"" +
						list[variadic_type].get_name() +
						"\" can't be variadic at the same time!";

					switch (cr.resolve(ctx)) {
					case pl::error::ResolutionStrategy::ApplyOptionA:
						std::cout << list[i].get_name()
							<< " toggle to non variadic type!\n";
						list[i].get_type().toggle_variadic();
						break;
					case pl::error::ResolutionStrategy::ApplyOptionB:
						std::cout << list[variadic_type].get_name()
							<< " toggle to non variadic type!\n";
						list[variadic_type].get_type().toggle_variadic();
						break;
					default:
						break;
					}

					variadic_type = -1;
					i = -1;
				}
			}
		}
		
		for (size_t i = 0; i < list.size(); i++) {
			if (list[i].get_type().is_variadic() && i < list.size() - 1) {
				CGContext ctx;
				ctx.type = CGError::VariadicDoNotOnTheEnd;
				ctx.message = "entity: \"" +
					list[i].get_name()
					+ "\" " + "would be on the end!";

				switch (cr.resolve(ctx)) {
				case pl::error::ResolutionStrategy::ApplyOptionA:
					std::cout << list[i].get_name()
						<< " moved to the end!\n";
					args.swap_args(i, list.size() - 1);
					break;
				case pl::error::ResolutionStrategy::ApplyOptionB:
					std::cout << list[i].get_name()
						<< " toggle to non variadic!\n";
					list[i].get_type().toggle_variadic();
					break;
				default:
					break;
				};
			}
		}
	}

	inline void FunctionValidator::verify(source::Function& func) {
		std::vector<source::TypeName> req_list;

		for (const auto& ns : func.get_namespace()) { req_list.push_back(ns.get_name()); }
		for (const auto& arg : func.get_args()) { req_list.push_back(arg.get_type()); }
		req_list.push_back(func.get_type());

		temp_valid.verify(func);

		template_bind_validate(req_list, func);

		ArguementValidator().verify(func);
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

	inline bool Validator::template_bind_validate(
		std::vector<source::TypeName>& required_list,
		source::TemplateEntity& local)
	{
		bool conflict_resolved = false;

		std::vector<source::TypeName> recursive_required_templates;
		for (const auto& req_type : required_list) {
			recursive_template_collector(req_type, recursive_required_templates);
		}

		auto& declared_local_list = local.get_template_parametrs();

		for (const auto& require_template : recursive_required_templates) {
			if (!require_template.is_template()) continue;

			bool found = false;

			for (auto& declared_template : declared_local_list) {
				if (declared_template.get_name() == require_template.get_name()) {
					found = true;

					// Проверяем соответствие вариадиков
					if (declared_template.is_variadic() != require_template.is_variadic()) {
						CGContext ctx;
						ctx.type = CGError::MissMatchVariadic;

						std::string declared_status = declared_template.is_variadic() ? "" : "non-";
						std::string required_status = require_template.is_variadic() ? "" : "non-";

						ctx.message = "Template parameter \"" + require_template.get_name() +
							"\" variadic mismatch: declared as " + declared_status +
							"variadic but required as " + required_status + "variadic";

						switch (cr.resolve(ctx)) {
						case pl::error::ResolutionStrategy::ApplyOptionA:
							// Только один вариант: меняем declared
							std::cout << declared_template.get_name()
								<< " toggled to "
								<< (require_template.is_variadic() ? "" : "non-")
								<< "variadic!\n";
							declared_template.toggle_variadic();
							conflict_resolved = true;
							break;

						default:
							// Игнорируем
							break;
						}
					}

					break;
				}
			}

			// Если шаблон не найден - добавляем
			if (!found) {
				CGContext ctx;
				ctx.type = CGError::TemplateTypeIsNotLinked_Local;
				ctx.message = "Template parameter \"" + require_template.get_name() +
					"\" is not declared in the local template list!";

				switch (cr.resolve(ctx)) {
				case pl::error::ResolutionStrategy::ApplyOptionA:
					std::cout << "Adding template parameter \"" << require_template.get_name()
						<< "\" to local list.\n";
					local.add_template_parametr(require_template);
					conflict_resolved = true;
					break;

				default:
					break;
				}
			}
		}

		return conflict_resolved;
	}

	inline bool Validator::template_bind_validate(
		std::vector<source::TypeName>& required_list,
		source::TemplateEntity& local,
		source::TemplateEntity& external)
	{
		bool conflict_resolved = false;

		std::vector<source::TypeName> recursive_required_templates;
		for (const auto& req_type : required_list) {
			recursive_template_collector(req_type, recursive_required_templates);
		}

		auto& declared_local_list = local.get_template_parametrs();

		for (const auto& require_template : recursive_required_templates) {
			if (!require_template.is_template()) continue;

			bool found = false;

			// Сначала ищем во внешнем шаблоне
			for (auto& class_template : external.get_template_parametrs()) {
				if (class_template.get_name() == require_template.get_name()) {
					found = true;

					// Проверяем соответствие вариадиков
					if (class_template.is_variadic() != require_template.is_variadic()) {
						CGContext ctx;
						ctx.type = CGError::MissMatchVariadic;

						std::string declared_status = class_template.is_variadic() ? "" : "non-";
						std::string required_status = require_template.is_variadic() ? "" : "non-";

						ctx.message = "Template parameter \"" + require_template.get_name() +
							"\" variadic mismatch in external template: declared as " +
							declared_status + "variadic but required as " +
							required_status + "variadic";

						switch (cr.resolve(ctx)) {
						case pl::error::ResolutionStrategy::ApplyOptionA:
							// Меняем external
							std::cout << class_template.get_name()
								<< " toggled to "
								<< (require_template.is_variadic() ? "" : "non-")
								<< "variadic in external template!\n";
							class_template.toggle_variadic();
							conflict_resolved = true;
							break;

						default:
							break;
						}
					}

					break;
				}
			}

			// Если не нашли во внешнем - ищем в локальном
			if (!found) {
				for (auto& method_template : declared_local_list) {
					if (method_template.get_name() == require_template.get_name()) {
						found = true;

						// Проверяем соответствие вариадиков
						if (method_template.is_variadic() != require_template.is_variadic()) {
							CGContext ctx;
							ctx.type = CGError::MissMatchVariadic;

							std::string declared_status = method_template.is_variadic() ? "" : "non-";
							std::string required_status = require_template.is_variadic() ? "" : "non-";

							ctx.message = "Template parameter \"" + require_template.get_name() +
								"\" variadic mismatch in local template: declared as " +
								declared_status + "variadic but required as " +
								required_status + "variadic";

							switch (cr.resolve(ctx)) {
							case pl::error::ResolutionStrategy::ApplyOptionA:
								// Меняем local
								std::cout << method_template.get_name()
									<< " toggled to "
									<< (require_template.is_variadic() ? "" : "non-")
									<< "variadic in local template!\n";
								method_template.toggle_variadic();
								conflict_resolved = true;
								break;

							default:
								break;
							}
						}

						break;
					}
				}
			}

			// Если шаблон не найден нигде
			if (!found) {
				CGContext ctx;
				ctx.type = CGError::TemplateTypeIsNotLinked_External;
				ctx.message = "Template parameter \"" + require_template.get_name() +
					"\" is not declared in any template list!";

				switch (cr.resolve(ctx)) {
				case pl::error::ResolutionStrategy::ApplyOptionA:
					std::cout << "Adding template parameter \"" << require_template.get_name()
						<< "\" to local template list.\n";
					local.add_template_parametr(require_template);
					conflict_resolved = true;
					break;

				case pl::error::ResolutionStrategy::ApplyOptionB:
					std::cout << "Adding template parameter \"" << require_template.get_name()
						<< "\" to external template list.\n";
					external.add_template_parametr(require_template);
					conflict_resolved = true;
					break;

				default:
					break;
				}
			}
		}

		return conflict_resolved;
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
		for (auto& t : type.get_namespace()) recursive_template_collector(t.get_name(), accumulator);

		// А теперь идем вглубь по всем вложенным шаблонным аргументам
		// Например, для vec_type<Tag> мы зайдем в Tag
		// Для std::map<K, V> мы зайдем и в K, и в V
		for (const auto& nested_template_arg : type.get_template_parametrs()) {
			recursive_template_collector(nested_template_arg, accumulator);
		}
	}

	inline void TemplateParamValidator::verify(source::TemplateEntity& entity) {
		using namespace pl::error;

		auto& list = entity.get_template_parametrs();

		int variadic_type = -1;

		for (size_t i = 0; i < list.size(); i++) {
			if (list[i].is_variadic()) {
				if (variadic_type == -1) {
					variadic_type = i;
				}
				else {
					CGContext ctx;
					ctx.type = CGError::MuchVariadicTemplates;
					ctx.message = "entity: \"" +
						list[i].get_name() +
						"\" and \"" +
						list[variadic_type].get_name() +
						"\" can't be variadic at the same time!";

					switch (cr.resolve(ctx)) {
					case ResolutionStrategy::ApplyOptionA:
						std::cout << list[i].get_name()
							<< " toggle to non variadic type!\n";
						list[i].toggle_variadic();
						break;
					case ResolutionStrategy::ApplyOptionB:
						std::cout << list[variadic_type].get_name()
							<< " toggle to non variadic type!\n";
						list[variadic_type].toggle_variadic();
						break;
					default:
						break;
					}
					
					variadic_type = -1;
					i = -1;
				}
			}
		}

		for (size_t i = 0; i < list.size(); i++) {
			if (list[i].is_variadic() && i < list.size() - 1) {
				CGContext ctx;
				ctx.type = CGError::VariadicDoNotOnTheEnd;
				ctx.message = "entity: \"" +
					list[i].get_name()
					+ "\" " + "would be on the end!";

				switch (cr.resolve(ctx)) {
				case ResolutionStrategy::ApplyOptionA:
					std::cout << list[i].get_name()
					<< " moved to the end!\n";
					entity.swap_templates(i, list.size() - 1);
					break;
				case ResolutionStrategy::ApplyOptionB:
					std::cout << list[i].get_name()
					<< " toggle to non variadic!\n";
					list[i].toggle_variadic();
					break;
				default:
					break;
				};
			}
		}

		for (auto& type : list) {
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