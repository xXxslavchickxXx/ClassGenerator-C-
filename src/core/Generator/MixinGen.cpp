#include <Generator/MixinGen.h>

#include <sstream>
#include <format>
#include <Entity/Entities.h>

namespace cg::gen {
	void MixinGenerator::initialize() {
		is_init = true;
		disp.registry(std::make_unique<TypeGenerator>());
	}

	bool MixinGenerator::is_init = false;
	ValueDispatcher MixinGenerator::disp;

	std::string MixinGenerator::generate(const src::ITreeElement* obj,
										 const src::ITreeElement* ctx) {
		if (!is_init) initialize();

		return disp.generate(obj, ctx);
	}

	void ValueDispatcher::registry(std::unique_ptr<IValueGenerator> generator) {
		if (!generator) return;

		const auto& id = generator->get_id();
		if (has_generator(id)) return;

		generators[id] = std::move(generator);
	}
	void ValueDispatcher::unregistry(const std::string& id)
	{
		const auto& it = generators.find(id);

		if (it == generators.end()) return;

		generators.erase(it);
	}

	std::string ValueDispatcher::generate(const src::ITreeElement* obj,
										  const src::ITreeElement* ctx) const {
		for (const auto& [name, gen] : generators) {
			if (gen->can_generate(obj)) {
				return gen->generate(obj, ctx);
			}
		}

		std::string entity_name = "entity type";

		if (obj->as<src::NamedEntity>())
			entity_name = obj->as<src::NamedEntity>()->get_name();

		throw std::runtime_error(
			std::format("No generator found for {0}", entity_name));
	}
	bool ValueDispatcher::has_generator(const std::string& id) const {
		return generators.find(id) != generators.end();
	}

	bool TypeGenerator::can_generate(const src::ITreeElement* node) const {
		return node->as<ent::Type>() != nullptr;
	}
	std::string TypeGenerator::get_id() const {
		return "type";
	}
	std::string QualificatorGen::generate(const src::Qualificator& qual) {
		if (!qual.has_qualificator()) return "";

		std::stringstream sstr;

		if (qual.is_const()) sstr << " const";

		switch (qual.get_qualificator()) {
			case src::QUALIFICATOR::POINTER:
				sstr << "*";
				break;
			case src::QUALIFICATOR::REFERENCE:
				sstr << "&";
				break;
			case src::QUALIFICATOR::UNIVERSAL_REFERENCE:
				sstr << "&&";
				break;
		}

		return sstr.str();
	}
	std::string TypeGenerator::generate(const src::ITreeElement* obj,
										const src::ITreeElement* ctx) const
	{
		if (!obj) return "";
		
		const auto* type_ptr = obj->as<ent::Type>();

		std::stringstream sstr;

		if (type_ptr->is_const()) sstr << "const ";

		sstr << NamespaceGeter::generate_namespace(obj, ctx, true);
		sstr << QualificatorGen::generate(type_ptr->get_type_qualificator());

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

	bool is_named(const src::Node* obj) {
		return obj && obj->as<src::NamedEntity>() != nullptr;
	}
	bool is_template(const src::Node* obj) {
		return false;//obj->as<const src::>() != nullptr;
	}

	bool NamespaceGeter::same_path(const src::Node* first, const src::Node* second) {
		if (!first || !second) {
			return first == second;
		}

		if (!is_named(first) || !is_named(second)) return first == second;

		if (generate_namespace_prefix(first) !=
			generate_namespace_prefix(second)) return false;

		return same_path(first->get_parent(), second->get_parent());
	}

	size_t
		NamespaceGeter::relative_depth_count(const src::Node* obj,
											 const src::Node* ctx) {
		if (!obj && ctx) {
			throw std::runtime_error(
				std::format("Cannot generate definition in namespace '{}' from current context '{}': "
					"namespaces are independent and not nested. Use explicit qualification.",
					generate_namespace_prefix(ctx), "")
			);
		}

		const src::Node* iter = obj;
		size_t depth = 0;
		bool found_ctx = false;

		while (iter) {
			if (ctx) {
				if (generate_namespace_prefix(ctx) ==
					generate_namespace_prefix(iter)) {
					if (!same_path(iter, ctx)) {
						throw std::runtime_error(
							std::format("Cannot generate definition in namespace '{}' from current context '{}': "
								"namespaces are independent and not nested. Use explicit qualification.",
								generate_namespace_prefix(ctx),
								generate_namespace_prefix(iter))
						);
					}
					found_ctx = true;
					break;
				}
			}
			iter = iter->get_parent();
			depth++;
		}

		if (ctx && !found_ctx) {
			throw std::runtime_error(
				std::format("Cannot generate definition in namespace '{}' from current context '{}': "
					"namespaces are independent and not nested. Use explicit qualification.",
					generate_namespace_prefix(ctx), generate_namespace_prefix(obj))
			);
		}

		return depth;
	}

	std::vector<const src::Node*>
		NamespaceGeter::relative_path(const src::Node* obj,
											const src::Node* ctx)
	{
		std::vector<const src::Node*> name_list;

		size_t path_depth = relative_depth_count(obj, ctx);

		auto* iter = obj;

		for (size_t i = 0; i < path_depth; i++) {
			name_list.push_back(iter);
			iter = iter->get_parent();
		}

		std::reverse(name_list.begin(), name_list.end());
		return name_list;
	}

	std::string NamespaceGeter::generate_namespace(const src::ITreeElement* obj,
												   const src::ITreeElement* ctx,
												   bool typename_declaration)
	{
		std::stringstream sstr;

		auto get_node_ptr = [](const auto* ptr) -> const src::Node* {
			if (ptr->as<src::Node>())
				return ptr->as<src::Node>();
			else if (ptr->as<ent::Reference>())
				return ptr->as<ent::Reference>()->get_target();

			return nullptr;
		};

		const src::Node* obj_node = get_node_ptr(obj);
		const src::Node* ctx_node = get_node_ptr(ctx);

		auto ns_list = relative_path(obj_node, ctx_node);

		//bool has_dependent;

		if (!ns_list.size()) return "";

		for (size_t i = 0; i < ns_list.size(); i++) {
			const auto& ns = ns_list[i];
			sstr << generate_namespace_prefix(ns);
			
			if (i != ns_list.size() - 1) sstr << "::";
		}

		return sstr.str();
	}

	std::string NamespaceGeter::generate_namespace_prefix(const src::Node* obj) {
		if (!obj || !is_named(obj))
			throw std::runtime_error("Object would be a named_entity!");

		std::stringstream sstr;

		sstr << obj->as<src::NamedEntity>()->get_name();

		if (is_template(obj))
		;// ToDo когда добавлю шаблонные сущности

		return sstr.str();
	}
}