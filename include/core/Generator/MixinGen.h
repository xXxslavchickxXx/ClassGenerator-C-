#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace cg::src {
	class Node;
	class ITreeElement;
	class Qualificator;
}

namespace cg::gen {
	std::string tabulate(const std::string& text, size_t count,
			const std::string& indent = "    ");
	
	bool is_named(const src::Node* obj);
	bool is_template(const src::Node* obj);

	class IValueDispacther {
	public:
		virtual std::string generate(const src::ITreeElement* obj,
									 const src::ITreeElement* ctx) const = 0;
	};

	class IValueGenerator {
	public:
		virtual bool can_generate(const src::ITreeElement* node) const = 0;
		virtual std::string get_id() const = 0;
		virtual std::string generate(const src::ITreeElement* obj,
									 const src::ITreeElement* ctx) const = 0;
	};

	class ValueDispatcher
		: public IValueDispacther {
		std::unordered_map<std::string, std::unique_ptr<IValueGenerator>> generators;

	public:
		ValueDispatcher() = default;

		void registry(std::unique_ptr<IValueGenerator> generator);
		void unregistry(const std::string& id);

		std::string generate(const src::ITreeElement* obj,
							 const src::ITreeElement* ctx) const override;

		bool has_generator(const std::string& id) const;
	};

	class TypeGenerator
		: public IValueGenerator {

	public:
		TypeGenerator() = default;

		bool can_generate(const src::ITreeElement* node) const override;
		std::string get_id() const override;
		std::string generate(const src::ITreeElement* obj,
							 const src::ITreeElement* ctx) const override;

	};

	class MixinGenerator {
		static ValueDispatcher disp;

		static bool is_init;
		static void initialize();
	public:
		static std::string generate(const src::ITreeElement* obj,
									const src::ITreeElement* ctx = nullptr);

	};

	struct InstanceGen {

	};

	struct QualificatorGen {
		static std::string generate(const src::Qualificator& qual);
	};

	class NamespaceGeter {
	public:
		static std::vector<const src::Node*>
			relative_path(const src::Node* obj,
								const src::Node* ctx = nullptr);

		static std::string generate_namespace(const src::ITreeElement* obj,
											  const src::ITreeElement* ctx = nullptr,
											  bool typename_declaration = false);

		static std::string generate_namespace_prefix(const src::Node* obj);

	private:
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