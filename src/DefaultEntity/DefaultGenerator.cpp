#include <DefaultEntity/DefaultGenerator.h>

#include <AccessEntity/AccessGenerator.h>

namespace cg::generate {
	std::string tabulate(size_t count, const std::string& str) {
		std::string temp_str;

		std::stringstream sstr(str);
		std::string token;
		while (std::getline(sstr, token, '\n')) {
			std::string space;
			for (size_t i = 0; i < count; i++) space += "    ";
			temp_str +=
				space
				+ token;

			if (!sstr.eof()) {
				temp_str += '\n';
			}
		}

		return temp_str;
	}

	std::string
		NamespaceGenerator::generate(
			const cgs::Namespace& n, bool realization)
	{
		std::stringstream sstr;

		sstr << "namespace "
		<< n.get_name().get_name();

		sstr << " {\n";

		// Генерация всех сущностей в порядке добавления
		for (const auto& entity : n.get_entities()) {
			std::visit(overloaded{
				[&](const cgs::Variable& v) {
					std::string code =
						VariableGenerator::generate(v, realization);
					if (!code.empty()) {
						sstr << tabulate(1, code) << "\n";
					}
				},
				[&](const cgs::Alias& a) {
					std::string code = AliasGenerator::generate(a);
					if (!code.empty()) {
						sstr << tabulate(1, code) << "\n";
					}
				},
				[&](const cgs::Function& m) {
					std::string code =
						FunctionGenerator::generate(m, realization);
					if (!code.empty()) {
						sstr << tabulate(1, code) << "\n";
					}
				},
				[&](const cgs::Class& cls) {
					std::string code =
						ClassGenerator::generate(cls, realization);
					if (!code.empty()) {
						sstr << tabulate(1, code) << "\n";
					}
				},
				[&](const cgs::Namespace& ns) {
					std::string code = generate(ns, realization);
					if (!code.empty()) {
						sstr << tabulate(1, code) << "\n";
					}
				}
				}, entity);
		}

		sstr << "}";

		return sstr.str();
	}

	std::string FunctionGenerator::generate(
	const cgs::Function& f, bool realization) {
		std::stringstream sstr;

		// Условия скипа генерации
		if (realization)
			if (f.is_constexpr() || f.is_inline()) {
				return "";
			}

		bool is_template = !f.get_template_parametrs().empty();

		if (is_template)
			sstr << TemplateEntityGenerator::generate(f, false) << '\n';


		if (is_template && realization && !f.is_inline()) {
			auto f_inline = f;
			f_inline.toggle_inline();
			sstr << OptionalEntityGenerator::generate(f_inline, realization);
		}
		else
			sstr << OptionalEntityGenerator::generate(f, realization);

		sstr << ArgumentableEntityGenerator::generate(f, !realization);
		if (f.is_inline() || f.is_constexpr() ||
			!f.has_definition() || realization)
				sstr << " {\n" << tabulate(1, "//TODO...") << "\n}";
		else sstr << ";";
		return sstr.str();

		return sstr.str();
	}

	std::string ArgumentGenerator::generate(const cgs::Arguement& a, bool value) {
		std::stringstream sstr;

		sstr << TypeNameGenerator::generate(a.get_type()) << " ";
		sstr << NamedGenerator::generate(a);
		if (!a.get_value().empty() && value) {
			sstr << " = " << a.get_value();
		}
		return sstr.str();

		return sstr.str();
	}

	std::string ArgumentableEntityGenerator::generate(const cgs::ArgumentableEntity& a, bool values) {
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

	std::string VariableGenerator::generate(
	const cgs::Variable& v, bool realization) {
		std::stringstream sstr;

		if (realization) {
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
		if (!realization && is_global) {
			sstr << "extern ";
		}

		sstr << OptionalEntityGenerator::generate(v, realization);

		if (!v.get_value().empty()) {
			sstr << " = " << v.get_value();
		}

		sstr << ";";

		return sstr.str();
	}

	std::string OptionalEntityGenerator::generate(
	const cgs::OptionalEntity& t, bool realization) {
		std::stringstream sstr;

		if (t.is_friend() && !realization) sstr << "friend ";
		if (t.is_static() && !realization) sstr << "static ";
		if (t.is_constexpr()) sstr << "constexpr ";
		if (t.is_inline() && !realization) sstr << "inline ";

		sstr << TypeNameGenerator::generate(t.get_type()) << " ";
		sstr << NamedGenerator::generate(t);

		return sstr.str();
	}

	std::string TemplateEntityGenerator::generate(const cgs::TemplateEntity& t, bool arguments) {
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

	std::string TypeNameGenerator::generate(
		const cgs::TypeName& t)
	{
		std::stringstream sstr;

		if (t.is_const()) sstr << "const ";

		bool dependent = NamespacePrefixGenerator::dependent_path(t);

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

	std::string NamedGenerator::generate(
		const cgs::NamedEntity& n, bool template_entity)
	{
		std::stringstream sstr;

		sstr << NamespacePrefixGenerator::generate(n);
		if (template_entity) sstr << "template ";
		sstr << n.get_name();

		return sstr.str();
	}

	std::string NamespacePrefixGenerator::generate(
		const cgs::NamedEntity& n)
	{
		const std::vector<cgs::Namespace>& pref = n.get_namespace();
		if (pref.empty()) return "";

		std::stringstream sstr;
		for (auto& prefix : pref) {
			if (!prefix.get_name().get_name().empty())
				sstr
				<< NamedGenerator::generate(prefix.get_name(),
					!prefix.get_name().get_template_parametrs().empty())
				<< TemplateEntityGenerator::generate(prefix.get_name(), true)
				<< "::";
		}

		return sstr.str();
	}
	std::string NamespacePrefixGenerator::generate(
		const cgs::Namespace& n)
	{
		return generate(cg::build::TypeBuilder("")
			.ns(n.get_name())
			.build());
	}
	bool NamespacePrefixGenerator::dependent_path(const cgs::NamedEntity& n)
	{
		const std::vector<cgs::Namespace>& pref = n.get_namespace();
		for (auto& prefix : pref) {
			if (prefix.get_name().is_template()) return true;
		}
		return false;
	}
}