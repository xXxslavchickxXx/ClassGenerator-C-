#include <Generator/EntitiesGen.h>

#include <Entity/Entities.h>
#include <Generator/MixinGen.h>
#include <vector>
#include <sstream>

namespace cg::gen {
	std::string generate(const src::Node* raw_node,
		bool declaration,
		const src::Node* ctx)
	{
		if (raw_node->as<ent::Namespace>())
			return generate(raw_node->as<ent::Namespace>(), declaration, ctx);
	}

	std::string generate(const ent::Namespace* ns,
		bool declration,
		const src::Node* ctx)
	{
		std::stringstream sstr;

		sstr << "namespace ";

		const auto* iter = ns;
		while (true)
			if (iter->size() == 1 &&
				iter->get_child(0)->as<ent::Namespace>())
				iter = iter->get_child(0)->as<ent::Namespace>();
			else break;

		sstr << NamespaceGeter::generate_namespace(iter, ctx);

		sstr << " {\n";

		for (size_t i = 0; i < iter->size(); i++) {
			sstr
				<< tabulate(generate(iter->get_child(0), declration, iter), 1);
			
			if (i < iter->size() - 1)
				sstr 
					<< "\n\n";
		}

		sstr << "}";

		return sstr.str();
	}
}