#include <Generator/MixinGen.h>

#include <sstream>

namespace cg::gen {
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