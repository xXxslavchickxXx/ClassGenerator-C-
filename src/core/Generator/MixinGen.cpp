#include <Generator/MixinGen.h>

#include <sstream>
#include <format>
#include <Entity/Entities.h>

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

	std::string NamespaceGeter::generate_namespace(const src::Node* obj,
		const src::Node* ctx)
	{
		std::stringstream sstr;

		auto ns_list = relative_path(obj, ctx);

		if (!ns_list.size()) return "";

		for (size_t i = 0; i < ns_list.size(); i++) {
			const auto& ns = ns_list[i];
			sstr << generate_namespace_prefix(ns);
			
			if (i != ns_list.size() - 1) sstr << "::";
		}

		return sstr.str();
	}

	std::string NamespaceGeter::generate_namespace_prefix(const src::Node* obj) {
		if (!obj || !is_named(obj)) return "";

		std::stringstream sstr;

		sstr << obj->as<src::NamedEntity>()->get_name();

		if (is_template(obj))
		;// ToDo когда добавлю шаблонные сущности

		return sstr.str();
	}
}