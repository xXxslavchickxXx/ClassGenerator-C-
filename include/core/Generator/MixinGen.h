#pragma once

#include <string>
#include <vector>

namespace cg::src {
	class Node;
}

namespace cg::gen {
	std::string tabulate(const std::string& text, size_t count,
			const std::string& indent = "    ");
	
	bool is_named(const src::Node* obj);
	bool is_template(const src::Node* obj);

	class NamespaceGeter {
	public:
		static std::vector<const src::Node*>
			relative_path(const src::Node* obj,
								const src::Node* ctx = nullptr);

		static std::string generate_namespace(const src::Node* obj,
									   const src::Node* ctx = nullptr);

		static std::string generate_namespace_prefix(const src::Node* obj);

	//private:
		/// <summary>
		/// @brief функция для поиска на сколько нужно глубоко взять путь
		/// в зависимости от контекста
		/// 
		/// @details она кинет исключение если относительного пути в контексте нет
		/// </summary>
		/// <param name="obj"></param>
		/// <param name="ctx"></param>
		/// <returns></returns>
		static size_t relative_depth_count(const src::Node* obj,
									 const src::Node* ctx);

		/// <summary>
		/// @brief функции сравнения похожи пути или нет
		/// 
		/// @details она спускается в самый низ до нулевого указателя,
		/// так как поиск просто похожего пути бесполезно, все равно определять
		/// вне чисто оригинального нейспейса ничего нельзя по стандарту с++
		/// </summary>
		/// <param name="first"></param>
		/// <param name="second"></param>
		/// <returns></returns>
		static bool same_path(const src::Node* first, const src::Node* second);
	};
}