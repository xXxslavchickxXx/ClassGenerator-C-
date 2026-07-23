#pragma once

#include <ErrorEvents.h>
#include <DefaultEntity/DefaultValidator.h>
#include <AccessEntity/AccessEntity.h>

namespace cg::validate {
/// Объявления
	class FieldValidator : public Validator {
	public:
		void verify(source::Field& field, source::Class& class_);
	};

	class ConstructorValidator : public Validator {
		TemplateParamValidator temp_valid;

	public:
		void verify(source::Constructor& method, source::Class& class_);
	};

	class MethodValidator : public Validator {
		TemplateParamValidator temp_valid;

	public:
		void verify(source::Method& method, source::Class& class_);
	};

	class AliasValidator : public Validator {
		TemplateParamValidator temp_valid;

	public:
		void verify(source::Alias& alias, source::Class* class_ = nullptr);
	};

	class ClassValidator : public Validator {
		TemplateParamValidator tvalidator;

		FieldValidator fvalidator;
		MethodValidator mvalidator;
		ConstructorValidator cvalidator;
		AliasValidator avalidator;

	public:
		void verify(source::Class& class_);
	};


}

/// Реализации
namespace cg::validate {
	void AliasValidator::verify(source::Alias& alias, source::Class* class_) {
		std::vector<source::TypeName> req_list;
		for (const auto& ns : alias.get_namespace()) { req_list.push_back(ns.name); }
		req_list.push_back(alias.get_underlying_type());

		if (class_) template_bind_validate(req_list, alias, *class_);
		else template_bind_validate(req_list, alias);
	}

	void ClassValidator::verify(source::Class& class_) {
		tvalidator.verify(class_);

		for (auto field : class_.get_fields()) fvalidator.verify(field, class_);
		for (auto alias : class_.get_aliases()) avalidator.verify(alias, &class_);
		for (auto method : class_.get_methods()) mvalidator.verify(method, class_);
		for (auto constructor : class_.get_constructors()) cvalidator.verify(constructor, class_);
		for (auto base_class : class_.get_base_classes()) verify(base_class);
	}

	void FieldValidator::verify(source::Field& field, source::Class& class_) {
		if (field.is_static() && field.is_mutable()) {
			CGContext ctx;
			ctx.type = CGError::FieldMutableAndStatic;
			ctx.message = "entity: \"" +
				field.get_type().get_name()
				+ "\" " + "can't be static and mutable at the same time!";

			switch (cr.resolve(ctx)) {
				case pl::error::ResolutionStrategy::ApplyOptionA:
					field.toggle_mutable();
					break;
				case pl::error::ResolutionStrategy::ApplyOptionB:
					field.toggle_static();
					break;
				default:
					break;
			}
		}

		std::vector<source::TypeName> req_list;
		for (const auto& ns : field.get_namespace()) { req_list.push_back(ns.name); }
		req_list.push_back(field.get_type());

		template_bind_validate(req_list, class_);
	}

	void ConstructorValidator::verify(source::Constructor& constructor, source::Class& class_) {
		std::vector<source::TypeName> req_list;
		for (const auto& arg : constructor.get_args()) { req_list.push_back(arg.get_type()); }

		temp_valid.verify(constructor);

		template_bind_validate(req_list, constructor, class_);
	}

	void MethodValidator::verify(source::Method& method, source::Class& class_) {
		std::vector<source::TypeName> req_list;
		for (const auto& ns : method.get_namespace()) { req_list.push_back(ns.name); }
		for (const auto& arg : method.get_args()) { req_list.push_back(arg.get_type()); }
		req_list.push_back(method.get_type());

		temp_valid.verify(method);

		template_bind_validate(req_list, method, class_);
	}
}