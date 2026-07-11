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
			type.get_templates().push_back(t);
			return *this;
		}

		pls::TypeName build() { return std::move(type); }
	};

	class VariableBuilder {
		pls::TypeName type;
		std::string name;
		std::string value = "";
		pls::DeclSpecifiers specs = pls::DeclSpecifiers::None;
		std::vector<source::NamespacePrefix> ns_prefix = {};

	public:
		VariableBuilder(const std::string& var_name)
			: name(var_name), type(TypeBuilder("int").build()) {}

		VariableBuilder& with_type(pls::TypeName t) { type = std::move(t); return *this; }
		VariableBuilder& with_value(std::string val) { value = std::move(val); return *this; }
		VariableBuilder& ns(source::NamespacePrefix prefix) { ns_prefix.push_back(std::move(prefix)); return *this; }

		VariableBuilder& as_static() { specs = specs | pls::DeclSpecifiers::Static; return *this; }
		VariableBuilder& as_const() { specs = specs | pls::DeclSpecifiers::Const; return *this; }
		VariableBuilder& as_constexpr() { specs = specs | pls::DeclSpecifiers::ConstExpression; return *this; }

		pls::Variable build() {
			return pls::Variable(type, name, value, ns_prefix, specs);
		}
	};

	class FunctionBuilder {
		pls::TypeName type;
		std::string name;
		pls::DeclSpecifiers specs = pls::DeclSpecifiers::None;
		std::vector<source::NamespacePrefix> ns_prefix = {};

	public:
		FunctionBuilder(const std::string& var_name)
			: name(std::move(var_name)), type(TypeBuilder("void").build()) {}

		FunctionBuilder& with_type(pls::TypeName t) { type = std::move(t); return *this; }
		FunctionBuilder& ns(source::NamespacePrefix prefix) { ns_prefix.push_back(std::move(prefix)); return *this; }

		FunctionBuilder& as_static() { specs = specs | pls::DeclSpecifiers::Static; return *this; }
		FunctionBuilder& as_const() { specs = specs | pls::DeclSpecifiers::Const; return *this; }
		FunctionBuilder& as_constexpr() { specs = specs | pls::DeclSpecifiers::ConstExpression; return *this; }


		pls::Function build() {
			return pls::Function(type, name, ns_prefix, specs);
		}
	};
}