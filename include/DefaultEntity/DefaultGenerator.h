#pragma once

#include <DefaultEntity/DefaultEntity.h>
#include <sstream>
#include <functional>

namespace cg::generate {
	enum class GenStage {
		Declaration,
		Realization,
		Inline
	};

	inline std::string tabulate(size_t count, const std::string& str) {
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
		static std::string generate(const cgs::OptionalEntity& t, GenStage g);
	};

	class VariableGenerator {
	public:
		static std::string generate(const cgs::Variable& v, GenStage g);
	};

	class ArgumentGenerator {
	public:
		static std::string generate(const cgs::Arguement& a, bool value = true);
	};

	class ArgumentableEntityGenerator {
	public:
		static std::string generate(const cgs::ArgumentableEntity& a, bool values = true);
	};

	class FunctionGenerator {
	public:
		static std::string generate(const cgs::Function& f, GenStage g);
	};
}

namespace cg::generate {
	inline std::string FunctionGenerator::generate(const cgs::Function& f, GenStage g) {
		std::stringstream sstr;

		// Условия скипа генерации
		if (g == GenStage::Realization)
			if(f.is_constexpr() || f.is_inline()) {
				return "";
		}

		bool is_template = !f.get_template_parametrs().empty() || f.get_type().is_template();

		if (is_template)
			sstr << TemplateEntityGenerator::generate(f, false) << '\n';


		if (is_template && g == GenStage::Realization && !f.is_inline()) {
			auto f_inline = f;
			f_inline.toggle_inline();
			sstr << OptionalEntityGenerator::generate(f_inline, GenStage::Inline);
		}
		else
			sstr << OptionalEntityGenerator::generate(f, g);

		sstr << ArgumentableEntityGenerator::generate(f, g != GenStage::Realization);
		if (f.is_inline() || f.is_constexpr() || g != GenStage::Declaration) sstr << " {\n\t//TODO...\n}";
		else sstr << ";";
		return sstr.str();

		return sstr.str();
	}

	inline std::string ArgumentGenerator::generate(const cgs::Arguement& a, bool value) {
		std::stringstream sstr;

		sstr << TypeNameGenerator::generate(a.get_type()) << " ";
		sstr << NamedGenerator::generate(a);
		if (!a.get_value().empty() && value) {
			sstr << " = " << a.get_value();
		}
		return sstr.str();

		return sstr.str();
	}

	inline std::string ArgumentableEntityGenerator::generate(const cgs::ArgumentableEntity& a, bool values) {
		std::stringstream sstr;

		auto& args = a.get_args();

		sstr << "(";
		for (size_t i = 0; i < args.size(); i++) {
			if (i > 0) sstr << ", ";
			sstr << ArgumentGenerator::generate(args[i], values);
		}
		sstr << ")";

		return sstr.str();
	}

	inline std::string VariableGenerator::generate(const cgs::Variable& v, GenStage g) {
		std::stringstream sstr;

		if (g == GenStage::Realization) {
			if (v.is_static() || v.is_constexpr() || v.is_inline()) {
				return "";
			}

			sstr << TypeNameGenerator::generate(v.get_type()) << " ";
			sstr << NamedGenerator::generate(v);
			if (!v.get_value().empty()) {
				sstr << " = " << v.get_value();
			}

			sstr << ";";

			return sstr.str();
		}

		bool is_global = !v.is_static() && !v.is_inline() && !v.is_constexpr();
		if (g == GenStage::Declaration && is_global) {
			sstr << "extern ";
		}

		sstr << OptionalEntityGenerator::generate(v, g);

		if (!v.get_value().empty()) {
			sstr << " = " << v.get_value();
		}

		sstr << ";";

		return sstr.str();
	}

	inline std::string OptionalEntityGenerator::generate(const cgs::OptionalEntity& t, GenStage g) {
		std::stringstream sstr;

		if (t.is_friend() && g != GenStage::Realization) sstr << "friend ";
		if (t.is_static() && g != GenStage::Realization) sstr << "static ";
		if (t.is_constexpr()) sstr << "constexpr ";
		if (t.is_inline() && g != GenStage::Realization) sstr << "inline ";

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
				<< (templates[i].is_variadic() ? "... " : " ")
				<< NamedGenerator::generate(templates[i]);
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

		if (t.is_variadic()) sstr << "...";

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