#pragma once

#include <AccessEntity/AccessEntity.h>
#include <optional>
#include <vector>

namespace cg::build {
	namespace pls = cg::source;

	class AliasBuilder {
		pls::TypeName underlying_type;
		std::string name;
		std::vector<pls::NamespacePrefix> ns_prefix = {};
		pls::Access access = pls::Access::Private;

	public:
		AliasBuilder(const std::string& var_name, const pls::TypeName& type)
			: name(var_name), underlying_type(type) {}

		AliasBuilder& ns(pls::NamespacePrefix prefix) { ns_prefix.push_back(std::move(prefix)); return *this; }

		AliasBuilder& as_private() { access = pls::Access::Private; return *this; }
		AliasBuilder& as_public() { access = pls::Access::Public; return *this; }
		AliasBuilder& as_protected() { access = pls::Access::Protected; return *this; }

		pls::Alias build() {
			return pls::Alias(name, std::move(ns_prefix), underlying_type, access);
		}
	};

	class FieldBuilder {
		pls::TypeName type;
		std::string name;
		std::string value = "";
		pls::DeclSpecifiers specs = pls::DeclSpecifiers::None;
		std::vector<pls::NamespacePrefix> ns_prefix = {};
		pls::Access access = pls::Access::Private;

	public:
		FieldBuilder(const std::string& var_name)
			: name(var_name), type(TypeBuilder("int").build()) {}

		FieldBuilder& with_type(pls::TypeName t) { type = std::move(t); return *this; }
		FieldBuilder& with_value(std::string val) { value = std::move(val); return *this; }
		FieldBuilder& ns(pls::NamespacePrefix prefix) { ns_prefix.push_back(std::move(prefix)); return *this; }

		FieldBuilder& as_static() { specs = specs | pls::DeclSpecifiers::Static; return *this; }
		FieldBuilder& as_const() { specs = specs | pls::DeclSpecifiers::Const; return *this; }
		FieldBuilder& as_constexpr() { specs = specs | pls::DeclSpecifiers::ConstExpression; return *this; }

		FieldBuilder& as_private() { access = pls::Access::Private; return *this; }
		FieldBuilder& as_public() { access = pls::Access::Public; return *this; }
		FieldBuilder& as_protected() { access = pls::Access::Protected; return *this; }

		pls::Field build() {
			return pls::Field(std::move(type), name, std::move(value), std::move(ns_prefix), specs, access);
		}
	};

	class MethodBuilder {
		pls::TypeName type;
		std::string name;
		pls::DeclSpecifiers specs = pls::DeclSpecifiers::None;
		std::vector<pls::NamespacePrefix> ns_prefix = {};
		pls::Access access = pls::Access::Private;

	public:
		MethodBuilder(const std::string& var_name)
			: name(std::move(var_name)), type(TypeBuilder("void").build()) {}

		MethodBuilder& with_type(pls::TypeName t) { type = std::move(t); return *this; }
		MethodBuilder& ns(pls::NamespacePrefix prefix) { ns_prefix.push_back(std::move(prefix)); return *this; }

		MethodBuilder& as_static() { specs = specs | pls::DeclSpecifiers::Static; return *this; }
		MethodBuilder& as_inline() { specs = specs | pls::DeclSpecifiers::Inline; return *this; }
		MethodBuilder& as_const() { specs = specs | pls::DeclSpecifiers::Const; return *this; }
		MethodBuilder& as_constexpr() { specs = specs | pls::DeclSpecifiers::ConstExpression; return *this; }

		MethodBuilder& as_private() { access = pls::Access::Private; return *this; }
		MethodBuilder& as_public() { access = pls::Access::Public; return *this; }
		MethodBuilder& as_protected() { access = pls::Access::Protected; return *this; }

		pls::Method build() {
			return pls::Method(std::move(type), name, std::move(ns_prefix), specs, access);
		}
	};

	class ConstructorBuilder {
		pls::Access access = pls::Access::Private;
		bool default_ = false;
		bool delete_ = false;
		bool explicit_ = false;

	public:
		ConstructorBuilder(const pls::Access& access_ = pls::Access::Private) : access(access_) {}

		ConstructorBuilder& as_default() { default_ = true; return *this; }
		ConstructorBuilder& as_delete() { delete_ = true; return *this; }
		ConstructorBuilder& as_explicit() { explicit_ = true; return *this; }

		ConstructorBuilder& as_private() { access = pls::Access::Private; return *this; }
		ConstructorBuilder& as_public() { access = pls::Access::Public; return *this; }
		ConstructorBuilder& as_protected() { access = pls::Access::Protected; return *this; }

		pls::Constructor build() {
			auto constr = pls::Constructor(access);
			if (default_) constr.toggle_default();
			if (delete_) constr.toggle_delete();
			if (explicit_) constr.toggle_explicit();
			return constr;
		}
	};

	class DestructorBuilder {
		pls::Access access = pls::Access::Public;
		bool default_ = false;
		bool virtual_ = true;

	public:
		DestructorBuilder() = default;

		DestructorBuilder& as_default() { default_ = true; return *this; }
		DestructorBuilder& as_virtual(bool is_virtual = true) { virtual_ = is_virtual; return *this; }

		DestructorBuilder& as_private() { access = pls::Access::Private; return *this; }
		DestructorBuilder& as_public() { access = pls::Access::Public; return *this; }
		DestructorBuilder& as_protected() { access = pls::Access::Protected; return *this; }

		pls::Destructor build() {
			auto dtor = pls::Destructor(access);
			if (default_) dtor.toggle_default();
			if (!virtual_) dtor.toggle_virtual();
			return dtor;
		}
	};

	class ClassBuilder {
		std::string name;
		pls::Access access = pls::Access::Public;

		std::vector<pls::Field> fields;
		std::vector<pls::Method> methods;
		std::vector<pls::Alias> aliases;
		std::vector<pls::Constructor> constructors;
		std::optional<pls::Destructor> custom_destructor;
		std::vector<pls::TypeName> templates;
		std::vector<pls::Class> base_classes;

	public:
		ClassBuilder(const std::string& class_name) : name(class_name) {}

		ClassBuilder& as_private() { access = pls::Access::Private; return *this; }
		ClassBuilder& as_public() { access = pls::Access::Public; return *this; }
		ClassBuilder& as_protected() { access = pls::Access::Protected; return *this; }

		ClassBuilder& with_field(pls::Field f) { fields.push_back(std::move(f)); return *this; }
		ClassBuilder& with_method(pls::Method m) { methods.push_back(std::move(m)); return *this; }
		ClassBuilder& with_alias(pls::Alias a) { aliases.push_back(std::move(a)); return *this; }
		ClassBuilder& with_constructor(pls::Constructor c) { constructors.push_back(std::move(c)); return *this; }
		ClassBuilder& with_destructor(pls::Destructor d) { custom_destructor = std::move(d); return *this; }
		ClassBuilder& add_template_parametr(pls::TypeName t) { templates.push_back(std::move(t)); return *this; }
		ClassBuilder& add_base_class(pls::Class c) { base_classes.push_back(std::move(c)); return *this; }

		pls::Class build() {
			auto cls = pls::Class(name, access);

			for (auto& f : fields) cls.add_field(std::move(f));
			for (auto& m : methods) cls.add_method(std::move(m));
			for (auto& c : constructors) cls.get_constructors().push_back(std::move(c));
			for (auto& t : templates) cls.add_template_parametr(std::move(t));
			for (auto& c : base_classes) cls.add_base_class(std::move(c));

			if (custom_destructor) {
				cls.set_destructor(std::move(custom_destructor.value()));
			}

			return cls;
		}
	};
}