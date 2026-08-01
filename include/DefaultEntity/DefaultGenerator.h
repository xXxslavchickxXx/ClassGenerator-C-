#pragma once

#include <AccessEntity/AccessEntity.h>
#include <sstream>
#include <functional>

namespace cg::generate {
	enum class GenStage {
		Declaration,
		Realization,
		Inline
	};

	std::string tabulate(size_t count, const std::string& str);

	namespace cgs = cg::source;

	class NamespacePrefixGenerator {
	public:
		static bool dependent_path(const cgs::NamedEntity& n);
		static std::string generate(const cgs::NamedEntity& n);
		static std::string generate(const cgs::Namespace& n);
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

	class NamespaceGenerator {
	public:
		static std::string generate(const cgs::Namespace& n, GenStage g);
	};
}