#pragma once

#include <AccessEntity/AccessEntity.h>
#include <optional>
#include <vector>

namespace cg::build {
	namespace pls = cg::source;

	class AliasBuilder {
		pls::Alias alias;

	public:
		AliasBuilder(const std::string& var_name)
			: alias(var_name) {}

		AliasBuilder& ns(pls::NamespacePrefix prefix) { alias.add_namespace_prefix(std::move(prefix)); return *this; }
		AliasBuilder& underlying_type(pls::TypeName t) { alias.set_underlying_type(std::move(t)); return *this; }

		AliasBuilder& with_template(pls::TypeName t) { alias.add_template_parametr(std::move(t)); return *this; }

		AliasBuilder& as_private() { alias.set_visibility(pls::Access::Private); return *this; }
		AliasBuilder& as_public() { alias.set_visibility(pls::Access::Public); return *this; }
		AliasBuilder& as_protected() { alias.set_visibility(pls::Access::Protected); return *this; }

		pls::Alias build() {
			return alias;
		}
	};

	class FieldBuilder {
		pls::Field field;

	public:
		FieldBuilder(const std::string& var_name)
			: field(var_name) {}

		FieldBuilder& with_type(pls::TypeName t) { field.get_type() = std::move(t); return *this; }
		FieldBuilder& with_value(std::string val) { field.set_value(std::move(val)); return *this; }
		FieldBuilder& ns(pls::NamespacePrefix prefix) { field.add_namespace_prefix(std::move(prefix)); return *this; }

		FieldBuilder& as_static() { if (!field.is_static()) field.toggle_static(); return *this; }
		FieldBuilder& as_const() { if (!field.get_type().is_const()) field.get_type().toggle_const(); return *this; }
		FieldBuilder& as_constexpr() { if (!field.is_constexpr()) field.toggle_constexpr(); return *this; }
		FieldBuilder& as_inline() { if (!field.is_inline()) field.toggle_inline(); return *this; }
		FieldBuilder& as_mutable() { if (!field.is_mutable()) field.toggle_mutable(); return *this; }

		FieldBuilder& as_private() { field.set_visibility(pls::Access::Private); return *this; }
		FieldBuilder& as_public() { field.set_visibility(pls::Access::Public); return *this; }
		FieldBuilder& as_protected() { field.set_visibility(pls::Access::Protected); return *this; }

		pls::Field build() {
			return field;
		}
	};

	class MethodBuilder {
		pls::Method method;

	public:
		MethodBuilder(const std::string& var_name)
			: method(var_name) {}

		MethodBuilder& with_type(pls::TypeName t) { method.get_type() = std::move(t); return *this; }
		MethodBuilder& ns(pls::NamespacePrefix prefix) { method.add_namespace_prefix(std::move(prefix)); return *this; }

		MethodBuilder& add_argument(pls::Variable arg) { method.get_args().push_back(std::move(arg)); return *this; }
		MethodBuilder& with_template(pls::TypeName t) { method.add_template_parametr(std::move(t)); return *this; }

		MethodBuilder& as_static() { if (!method.is_static()) method.toggle_static(); return *this; }
		MethodBuilder& as_inline() { if (!method.is_inline()) method.toggle_inline(); return *this; }
		MethodBuilder& as_const() { if (!method.is_const()) method.toggle_const(); return *this; }
		MethodBuilder& as_constexpr() { if (!method.is_constexpr()) method.toggle_constexpr(); return *this; }

		MethodBuilder& as_virtual() { method.set_method_type(pls::MethodType::VIRTUAL); return *this; }
		MethodBuilder& as_abstract() { method.set_method_type(pls::MethodType::ABSOLUTE_VIRTUAL); return *this; }
		MethodBuilder& as_override() { method.set_method_type(pls::MethodType::OVERRIDE); return *this; }
		MethodBuilder& as_default() { method.set_method_type(pls::MethodType::DEFAULT); return *this; }
		MethodBuilder& as_deleted() { method.set_method_type(pls::MethodType::DELETED); return *this; }

		MethodBuilder& as_private() { method.set_visibility(pls::Access::Private); return *this; }
		MethodBuilder& as_public() { method.set_visibility(pls::Access::Public); return *this; }
		MethodBuilder& as_protected() { method.set_visibility(pls::Access::Protected); return *this; }

		pls::Method build() {
			return method;
		}
	};

	class ConstructorBuilder {
		pls::Constructor ctor;

	public:
		ConstructorBuilder() = default;

		ConstructorBuilder& add_argument(pls::Variable arg) { ctor.get_args().push_back(std::move(arg)); return *this; }
		ConstructorBuilder& add_init(pls::Field field) { ctor.add_init(std::move(field)); return *this; }
		ConstructorBuilder& with_template(pls::TypeName t) { ctor.add_template_parametr(std::move(t)); return *this; }

		ConstructorBuilder& as_default() { ctor.toggle_default(); return *this; }
		ConstructorBuilder& as_delete() { ctor.toggle_delete(); return *this; }
		ConstructorBuilder& as_explicit() { ctor.toggle_explicit(); return *this; }

		ConstructorBuilder& as_private() { ctor.set_visibility(pls::Access::Private); return *this; }
		ConstructorBuilder& as_public() { ctor.set_visibility(pls::Access::Public); return *this; }
		ConstructorBuilder& as_protected() { ctor.set_visibility(pls::Access::Protected); return *this; }

		pls::Constructor build() {
			return ctor;
		}
	};

	class DestructorBuilder {
		pls::Destructor dtor;

	public:
		DestructorBuilder() = default;

		DestructorBuilder& with_template(pls::TypeName t) { dtor.add_template_parametr(std::move(t)); return *this; }

		DestructorBuilder& as_default() { dtor.toggle_default(); return *this; }
		DestructorBuilder& as_virtual(bool is_virtual = true) {
			if (is_virtual && !dtor.is_virtual()) dtor.toggle_virtual();
			else if (!is_virtual && dtor.is_virtual()) dtor.toggle_virtual();
			return *this;
		}

		DestructorBuilder& as_private() { dtor.set_visibility(pls::Access::Private); return *this; }
		DestructorBuilder& as_public() { dtor.set_visibility(pls::Access::Public); return *this; }
		DestructorBuilder& as_protected() { dtor.set_visibility(pls::Access::Protected); return *this; }

		pls::Destructor build() {
			return dtor;
		}
	};

	class ClassBuilder {
		pls::Class cls;

	public:
		ClassBuilder(const std::string& class_name)
			: cls(class_name) {}

		ClassBuilder& as_private() { cls.set_visibility(pls::Access::Private); return *this; }
		ClassBuilder& as_public() { cls.set_visibility(pls::Access::Public); return *this; }
		ClassBuilder& as_protected() { cls.set_visibility(pls::Access::Protected); return *this; }

		ClassBuilder& with_field(pls::Field f) { cls.add_field(std::move(f)); return *this; }
		ClassBuilder& with_method(pls::Method m) { cls.add_method(std::move(m)); return *this; }
		ClassBuilder& with_alias(pls::Alias a) { cls.add_alias(std::move(a)); return *this; }
		ClassBuilder& with_constructor(pls::Constructor c) { cls.add_constructor(std::move(c)); return *this; }
		ClassBuilder& with_destructor(pls::Destructor d) { cls.set_destructor(std::move(d)); return *this; }

		ClassBuilder& with_template(pls::TypeName t) { cls.add_template_parametr(std::move(t)); return *this; }
		ClassBuilder& add_base_class(pls::Class c) { cls.add_base_class(std::move(c)); return *this; }

		pls::Class build() {
			return cls;
		}
	};
}