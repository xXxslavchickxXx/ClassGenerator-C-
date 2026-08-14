#include <Generator/EntitiesGen.h>

#include <Entity/Entities.h>
#include <Generator/MixinGen.h>
#include <vector>
#include <sstream>
#include <iostream>
#include <format>

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

		std::string entity_name = "entity type";

		if (obj->as<src::NamedEntity>())
			entity_name = obj->as<src::NamedEntity>()->get_name();

		throw std::runtime_error(
			std::format("No generator found for {0}", entity_name));
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
	const ent::Namespace*
		NamespaceGenerator::find_first_non_empty_ns(const src::Node* obj) const {
		if (!obj) return nullptr;
		const auto* iter = obj->as<ent::Namespace>();

		while (true) {
			if ((iter->size() == 1) &&
				(iter->get_child(0)->as<ent::Namespace>() != nullptr))
				iter = iter->get_child(0)->as<ent::Namespace>();
			else break;
		}

		return iter;
	}
	std::string NamespaceGenerator::generate(const src::Node* obj,
						 const src::Node* ctx,
						 bool declaration,
						 const IDispacther* dispatcher) const
	{
		if (!obj) return "";

		// Собираем информацию по состоянию окружения
		std::stringstream body_str;
		const auto* iter = find_first_non_empty_ns(obj);

		const auto namespace_str = NamespaceGeter::generate_namespace(iter, ctx);

		for (size_t i = 0; i < iter->size(); i++) {
			body_str
				<< dispatcher->generate(iter->get_child(i), iter, declaration)
				<< "\n";
			
			if (i < iter->size() - 1)
				body_str
					<< "\n";
		}

		// Заполняем главный поток в зависимости от полученных данных
		std::stringstream sstr;
		if (!namespace_str.empty()) {
			sstr << "namespace ";
			sstr << namespace_str;
			sstr << " {\n";

			sstr << tabulate(body_str.str(), 1);

			sstr << "}";
		}
		else sstr << body_str.str();

		return sstr.str();
	}

	bool ClassGenerator::can_generate(const src::Node* node) const {
		return node->as<ent::Class>() != nullptr;
	}
	std::string ClassGenerator::get_id() const {
		return "class";
	}
	std::string ClassGenerator::generate(const src::Node* obj,
											 const src::Node* ctx,
											 bool declaration,
											 const IDispacther* dispatcher) const
	{
		if (!obj) return "";

		const auto* cls = obj->as<ent::Class>();
		const auto* true_ctx = (declaration ? cls : ctx);

		std::stringstream body_str;
		for (size_t i = 0; i < cls->size(); i++) {
			body_str
				<< dispatcher->generate(cls->get_child(i), true_ctx, declaration)
				<< "\n";

			if (i < cls->size() - 1)
				body_str
				<< "\n";
		}

		std::stringstream main_str;

		if (declaration) {
			main_str << "class" << " " << cls->get_name();

			main_str << " {\n";

			main_str << tabulate(body_str.str(), 1);

			main_str << "}";
		}
		else main_str << body_str.str();

		return main_str.str();
	}
}