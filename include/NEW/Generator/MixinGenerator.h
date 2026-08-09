#pragma once

#include <string>
#include <Entity/Node.h>

namespace cg::gen {
	std::string
		tabulate(const std::string& text, size_t count,
			const std::string& indent = "    ");

	class NodeGenerator {
	public:
		static std::string
			generate(const std::shared_ptr<source::NodeEntity> ent,
				bool declaration = true);
	};

	class TemplateArgGenerator {
	public:
		static std::string
			generate(
				const std::shared_ptr<source::TemplateArgument> ent,
				bool declaration = true);
	};

	class TemplateGenerator {
	public:
		static std::string
			generate_list(
				const std::shared_ptr<source::TemplateEntity> ent,
				bool declaration = true);
	};
}