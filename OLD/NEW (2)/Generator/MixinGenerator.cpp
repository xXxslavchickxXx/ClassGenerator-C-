#include <Generator/MixinGenerator.h>
#include <Generator/AccessGenerator.h>

#include <sstream>

namespace cg::gen {
	std::string
		NodeGenerator::generate(
			const std::shared_ptr<source::NodeEntity> ent,
			bool declaration) {
		std::stringstream sstr;

		

		return sstr.str();
	}

	std::string
		TemplateArgGenerator::generate(
			const std::shared_ptr<source::TemplateArgument> ent,
			bool declaration) {
		std::stringstream sstr;

		if (ent->is_template_argument())
			sstr << "template<typename...> class";
		else if (ent->is_typename())
			sstr << "typename";
		else
			sstr << ClassGenerator::generate(ent->get_type(), false);

		return sstr.str();
	}

	std::string
		TemplateGenerator::generate_list(
			const std::shared_ptr<source::TemplateEntity> ent,
			bool declaration) {
		std::stringstream sstr;

		

		return sstr.str();
	}

	std::string
		tabulate(const std::string& text, size_t count,
			const std::string& indent) {
		// поток текста основного
		std::stringstream main_sstr(text);
		// результат
		std::stringstream tabulate_str;

		std::string line;

		while (std::getline(main_sstr, line, '\n')) {
			for (size_t i = 0; i < count; i++)
				tabulate_str << "    ";
			tabulate_str << line;
			if (!main_sstr.eof())
				tabulate_str << "\n";
		}

		return tabulate_str.str();
	}
}