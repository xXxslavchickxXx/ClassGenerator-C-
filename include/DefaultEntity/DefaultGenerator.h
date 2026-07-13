#pragma once

#include <DefaultEntity/DefaultEntity.h>
#include <sstream>
#include <functional>

namespace cg::generate {
	namespace cgs = cg::source;

	class Generator {
	protected:
		static std::string generate_template_args(const std::vector<cgs::TypeName>& templates);
	};

	class NamespaceGenerator : public Generator {
	public:
		static bool dependent_path(const cgs::NamedEntity& n);
		static std::string generate(const cgs::NamedEntity& n);
	};

	class NamedGenerator {
	public:
		static std::string generate(const cgs::NamedEntity& n, bool template_entity = false);
	};

	class TypeNameGenerator : public Generator {
	public:
		static std::string generate(const cgs::TypeName& t);
	};

	class TemplateEntityGenerator : public Generator {
		static std::function<void(const cgs::TypeName&)> non_temp;
	public:
		static std::string generate(const cgs::TemplateEntity& t, bool arguments = false);
	};
}

namespace cg::generate {
	inline std::function<void(const cgs::TypeName&)> non_temp = [](const cgs::TypeName&){
		
	};

	inline std::string TemplateEntityGenerator::generate(const cgs::TemplateEntity& t, bool arguments) {
		std::stringstream sstr;

		auto& templates = t.get_template_parametrs();

		if (arguments) {
			sstr << "<";
			for (size_t i = 0; i < templates.size(); i++) {
				if (i > 0) sstr << ", ";
				sstr << TypeNameGenerator::generate(templates[i]);
			}
			sstr << ">";
		}
		else {
			sstr << "template<";
			for (size_t i = 0; i < templates.size(); i++) {
				if (i > 0) sstr << ", ";
				non_temp(templates[i]);
				sstr << TypeNameGenerator::generate(templates[i].get_typename());
			}
			sstr << ">";
		}

		return sstr.str();
	}

	inline std::string TypeNameGenerator::generate(
		const cgs::TypeName& t)
	{
		std::stringstream sstr;

		if (t.is_const()) sstr << "const ";

		bool dependent = NamespaceGenerator::dependent_path(t);

		if (dependent) sstr << "typename ";
		sstr << NamedGenerator::generate(t, dependent && !t.get_templates().empty());
		sstr << generate_template_args(t.get_templates());

		switch (t.get_qualificator()) {
		case cg::source::Qualificator::Pointer:            sstr << "*";  break;
		case cg::source::Qualificator::Reference:          sstr << "&";  break;
		case cg::source::Qualificator::UniversalReference: sstr << "&&"; break;
		default: break;
		}

		return sstr.str();
	}

	inline std::string NamedGenerator::generate(
		const cgs::NamedEntity& n, bool template_entity)
	{
		std::stringstream sstr;
		
		sstr << NamespaceGenerator::generate(n);
		if (template_entity) sstr << "template ";
		sstr << n.get_name();

		return sstr.str();
	}

	inline std::string NamespaceGenerator::generate(
		const cgs::NamedEntity& n)
	{
		const std::vector<cgs::NamespacePrefix>& pref = n.get_namespace();
		if (pref.empty()) return "";

		std::stringstream sstr;
		for (auto& prefix : pref) {
			if (!prefix.name.get_name().empty()) 
				sstr
				<< NamedGenerator::generate(prefix.name, !prefix.name.get_templates().empty())
				<< generate_template_args(prefix.name.get_templates())
				<< "::";
		}

		return sstr.str();
	}
	inline bool NamespaceGenerator::dependent_path(const cgs::NamedEntity& n)
	{
		const std::vector<cgs::NamespacePrefix>& pref = n.get_namespace();
		for (auto& prefix : pref) { if (prefix.name.is_template()) return true; }
		return false;
	}

	inline std::string Generator::generate_template_args(const std::vector<cgs::TypeName>& templates) {
		if (templates.empty()) return "";

		std::stringstream sstr;
		sstr << "<";

		for (size_t i = 0; i < templates.size(); i++) {
			if (i > 0) sstr << ", ";
			sstr << TypeNameGenerator::generate(templates[i]);
		}

		sstr << ">";
		return sstr.str();
	}
}