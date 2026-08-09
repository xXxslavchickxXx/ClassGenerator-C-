#pragma once

#include <Entity/AccessEntities.h>

#include <string>

namespace cg::gen {

	class ClassGenerator {
	public:
		static std::string
			generate(const std::shared_ptr<source::Class> ent,
				bool declaration = true);
	};
}