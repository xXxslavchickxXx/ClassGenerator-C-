#pragma once

#include <DefaultEntity/DefaultEntity.h>
#include <sstream>
#include <functional>

namespace cg::generate {
	std::string tabulate(size_t count, const std::string& str) {
		std::string temp_str;

		std::stringstream sstr(str);
		std::string token;
		while (std::getline(sstr, token, '\n')) {
			temp_str += std::string(count, '\t') + token;

			if (!sstr.eof()) {
				temp_str += '\n';
			}
		}

		return temp_str;
	}

	namespace cgs = cg::source;

	class NamespaceGenerator {
	public:
		static bool dependent_path(const cgs::NamedEntity& n);
		static std::string generate(const cgs::NamedEntity& n);
	};

	class NamedGenerator {
	public:
		static std::string generate(const cgs::NamedEntity& n, bool template_entity = false);
	};

	class TypeNameGenerator {
	public:
		static std::string generate(const cgs::TypeName& t);
	};

	class TemplateEntityGenerator {
	public:
		static std::string generate(const cgs::TemplateEntity& t, bool arguments = false);
	};

	class OptionalEntityGenerator {
	public:
		static std::string generate(const cgs::OptionalEntity& t);
	};

	class VariableGenerator {
	public:
		static std::string generate(const cgs::Variable& v);
	};

	class ArgumentableEntityGenerator {
	public:
		static std::string generate(const cgs::ArgumentableEntity& a);
	};

	class FunctionGenerator {
	public:
		static std::string generate(const cgs::Function& f);
	};
}

namespace cg::generate {
	inline std::string FunctionGenerator::generate(const cgs::Function& f) {
		std::stringstream sstr;

		sstr << TemplateEntityGenerator::generate(f, false);
		if (!f.get_template_parametrs().empty()) sstr << "\n";

		sstr << OptionalEntityGenerator::generate(f)
		<< ArgumentableEntityGenerator::generate(f);

		return sstr.str();
	}

	inline std::string ArgumentableEntityGenerator::generate(const cgs::ArgumentableEntity& a) {
		std::stringstream sstr;

		auto& args = a.get_args();

		sstr << "(";
		for (size_t i = 0; i < args.size(); i++) {
			if (i > 0) sstr << ", ";
			sstr << VariableGenerator::generate(args[i]);
		}
		sstr << ")";

		return sstr.str();
	}

	inline std::string VariableGenerator::generate(const cgs::Variable& v) {
		std::stringstream sstr;

		sstr << OptionalEntityGenerator::generate(v);
		
		if (!v.get_value().empty()) sstr << " = " << v.get_value();

		return sstr.str();
	}

	inline std::string OptionalEntityGenerator::generate(const cgs::OptionalEntity& t) {
		std::stringstream sstr;

		if (t.is_constexpr()) sstr << "constexpr ";
		if (t.is_inline()) sstr << "inline ";
		if (t.is_constexpr()) sstr << "static ";

		sstr << TypeNameGenerator::generate(t.get_type()) << " ";
		sstr << NamedGenerator::generate(t);

		return sstr.str();
	}

	inline std::string TemplateEntityGenerator::generate(const cgs::TemplateEntity& t, bool arguments) {
		if (t.get_template_parametrs().empty()) return "";

		std::stringstream sstr;

		auto& templates = t.get_template_parametrs();

		if (templates.empty()) {
			throw std::invalid_argument("TemplateEntityGenerator: cannot generate from empty template parameters");
		}

		if (arguments) {
			sstr << "<";
			for (size_t i = 0; i < templates.size(); i++) {
				if (i > 0) sstr << ", ";
				sstr << TypeNameGenerator::generate(templates[i]);
			}
			sstr << ">";
		}
		else {
			for (const auto& param : templates) {
				if (param.get_typename().get_name().empty()) {
					throw std::invalid_argument("TemplateEntityGenerator: template parameter has no type name");
				}
			}
			sstr << "template<";
			for (size_t i = 0; i < templates.size(); i++) {
				if (i > 0) sstr << ", ";
				sstr << TypeNameGenerator::generate(templates[i].get_typename())
				<< " " << TypeNameGenerator::generate(templates[i]);
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
		sstr << NamedGenerator::generate(t, dependent && !t.get_template_parametrs().empty());
		sstr << TemplateEntityGenerator::generate(t, true);

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
				<< NamedGenerator::generate(prefix.name, !prefix.name.get_template_parametrs().empty())
				<< TemplateEntityGenerator::generate(prefix.name, true)
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
}