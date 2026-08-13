#include <Generator/EntitiesGen.h>

#include <Entity/Entities.h>
#include <Generator/MixinGen.h>
#include <vector>
#include <sstream>
#include <iostream>

namespace cg::gen {
	void GeneratorRegistrator::registry(std::unique_ptr<IGenerator> generator) {
		if (!generator) return;

		const auto& id = generator->get_id();
		if (has_generator(id)) return;

		generators[id] = std::move(generator);
	}
	void GeneratorRegistrator::unregistry(const std::string& id)
	{
		const auto& it = generators.find(id);

		if (it == generators.end()) return;

		generators.erase(it);
	}

	std::string GeneratorRegistrator::generate(const src::Node* obj,
											   const src::Node* ctx,
											   bool declaration) const {
		for (const auto& [name, gen] : generators) {
			if (gen->can_generate(obj)) {
				return gen->generate(obj, ctx, declaration, this);
			}
		}

		throw std::runtime_error("No generator found for this entity type");
	}
	bool GeneratorRegistrator::has_generator(const std::string& id) const {
		return generators.find(id) != generators.end();
	}

	bool NamespaceGenerator::can_generate(const src::Node* node) const {
		return node->as<ent::Namespace>() != nullptr;
	}
	std::string NamespaceGenerator::get_id() const {
		return "namespace";
	}
	std::string NamespaceGenerator::generate(const src::Node* obj,
						 const src::Node* ctx,
						 bool declaration,
						 const IDispacther* dispetcher) const {
		return "abrakadabra";
	}

	//std::string generate(const ent::Class* cls,
	//	const src::Node* ctx,
	//	bool declaration)
	//{
	//	std::stringstream sstr;

	//	sstr << "class" << " " << cls->get_name();

	//	sstr << " {\n";

	//	/*for (size_t i = 0; i < cls->size(); i++) {
	//		sstr
	//			<< tabulate(generate(cls->get_child(i), declaration, ctx), 1);
	//		
	//		if (i < cls->size() - 1)
	//			sstr 
	//				<< "\n\n";
	//	}*/

	//	sstr << "}";

	//	return sstr.str();
	//}
	//std::string generate(const ent::Namespace* ns,
	//	const src::Node* ctx,
	//	bool declaration)
	//{
	//	// Собираем информацию по состоянию окружения
	//	std::stringstream body_str;
	//	const auto* iter = ns;

	//	

	//	const auto namespace_str = NamespaceGeter::generate_namespace(iter, ctx);

	//	for (size_t i = 0; i < iter->size(); i++) {
	//		body_str
	//			<< generate(iter->get_child(i), iter, declaration)
	//			<< "\n";
	//		
	//		if (i < iter->size() - 1)
	//			body_str
	//				<< "\n";
	//	}

	//	// Заполняем главный поток в зависимости от полученных данных
	//	std::stringstream sstr;
	//	if (!namespace_str.empty()) {
	//		sstr << "namespace ";
	//		sstr << namespace_str;
	//		sstr << " {\n";

	//		sstr << tabulate(body_str.str(), 1);

	//		sstr << "}";
	//	}
	//	else sstr << body_str.str();

	//	return sstr.str();
	//}
}