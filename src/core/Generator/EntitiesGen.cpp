#include <Generator/EntitiesGen.h>

#include <Entity/Entities.h>
#include <sstream>

namespace cg::gen {
	std::string generate(const src::Node* raw_node,
		bool declaration,
		const src::Node* ctx)
	{
		if (raw_node->as<const ent::Namespace>())
			return generate(raw_node->as<const ent::Namespace>(), declaration, ctx);
	}

	std::string generate(const ent::Namespace* ns,
		bool declration,
		const src::Node* ctx)
	{
		std::stringstream sstr;



		return sstr.str();
	}
}