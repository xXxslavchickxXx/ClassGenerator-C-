#pragma once

#include <string>

namespace cg::gen {
	std::string
		tabulate(const std::string& text, size_t count,
			const std::string& indent = "    ");
}