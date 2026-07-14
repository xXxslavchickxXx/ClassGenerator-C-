#pragma once

#include <DefaultEntity/DefaultEntity.h>

namespace cg::build {
	namespace pls = cg::source;
	
	class TypeBuilder {
		pls::TypeName type;

	public:
		TypeBuilder(const std::string& name)
			: type(name) {}

		TypeBuilder(const pls::TypeName& t)
		: type(t) {}

		TypeBuilder& as_const() { if (!type.is_const()) type.toggle_const(); return *this; }
		TypeBuilder& as_template() { if (!type.is_template()) type.toggle_templatebale(); return *this; }
		TypeBuilder& as_variadic() { if (!type.is_variadic()) type.toggle_variadic(); return *this; }
		TypeBuilder& as_template(const pls::TypeName& t) { type.set_typename(t); return *this; }
		TypeBuilder& as_ptr() { type.set_qualificator(pls::Qualificator::Pointer); return *this; }
		TypeBuilder& as_ref() { type.set_qualificator(pls::Qualificator::Reference); return *this; }
		TypeBuilder& as_uni_ref() { type.set_qualificator(pls::Qualificator::UniversalReference); return *this; }
		TypeBuilder& ns(const source::NamespacePrefix& namespace_prefix) { type.add_namespace_prefix(namespace_prefix); return *this; }

		TypeBuilder& with_template(const pls::TypeName& t) {
			type.add_template_parametr(t);
			return *this;
		}

		pls::TypeName build() { return std::move(type); }
	};

	class VariableBuilder {
		pls::Variable var;

	public:
		VariableBuilder(const std::string& var_name)
			: var(var_name) {}

		VariableBuilder& with_type(pls::TypeName t) { var.get_type() = std::move(t); return *this; }
		VariableBuilder& with_value(std::string val) { var.set_value(std::move(val)); return *this; }
		VariableBuilder& ns(source::NamespacePrefix prefix) { var.add_namespace_prefix(std::move(prefix)); return *this; }

		VariableBuilder& as_static() { if (!var.is_static()) var.toggle_static(); return *this; }
		VariableBuilder& as_const() { if (!var.get_type().is_const()) var.get_type().toggle_const(); return *this; }
		VariableBuilder& as_constexpr() { if (!var.is_constexpr()) var.toggle_constexpr(); return *this; }

		pls::Variable build() {
			return var;
		}
	};

	class FunctionBuilder {
		pls::Function func;

	public:
		FunctionBuilder(const std::string& var_name)
			: func(var_name){}

		FunctionBuilder& with_type(pls::TypeName t) { func.get_type() = std::move(t); return *this; }
		FunctionBuilder& ns(source::NamespacePrefix prefix) { func.add_namespace_prefix(std::move(prefix)); return *this; }

		FunctionBuilder& as_static() { if (!func.is_static()) func.toggle_static(); return *this; }
		FunctionBuilder& as_inline() { if (!func.is_inline()) func.toggle_inline(); return *this; }
		FunctionBuilder& as_constexpr() { if (!func.is_constexpr()) func.toggle_constexpr(); return *this; }


		pls::Function build() {
			return func;
		}
	};
}