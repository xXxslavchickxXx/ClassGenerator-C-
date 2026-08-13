#pragma once

#include <string>
#include <memory>
#include <unordered_map>

namespace cg {
	namespace ent {
		class Namespace;
		class Variable;
		class Function;
		class Method;
		class Field;
		class Class;
	}

	namespace src {
		class Node;
	}
}

namespace cg::gen {
	
	class GeneratorRegistrator;

	class IDispacther {
	public:
		virtual std::string generate(const src::Node* obj,
									 const src::Node* ctx,
									 bool declaration) const = 0;
	};

	class IGenerator {
	public:
		virtual bool can_generate(const src::Node* node) const = 0;
		virtual std::string get_id() const = 0;
		virtual std::string generate(const src::Node* obj,
									 const src::Node* ctx,
									 bool declaration,
									 const IDispacther* dispetcher) const = 0;
	};

	class GeneratorRegistrator
		: public IDispacther {
		std::unordered_map<std::string, std::unique_ptr<IGenerator>> generators;

	public:
		void registry(std::unique_ptr<IGenerator> generator);
		void unregistry(const std::string& id);

		std::string generate(const src::Node* obj,
							 const src::Node* ctx = nullptr,
							 bool declaration = true) const override;

		bool has_generator(const std::string& id) const;
	};

	class NamespaceGenerator
		: public IGenerator
	{
		const src::Node* find_first_non_empty_ns(const src::Node* obj,
												 const src::Node* ctx);

	public:
		bool can_generate(const src::Node* node) const override;
		std::string get_id() const override;
		std::string generate(const src::Node* obj,
							 const src::Node* ctx,
							 bool declaration,
							 const IDispacther* dispetcher) const override;

	};

	/*class ClassGenerator
		: public IGenerator
	{
	public:
		bool can_generate(const src::Node* node) const override;
		std::string generate(const src::Node* obj,
							 const src::Node* ctx,
							 bool declaration,
							 const IDispecther* dispetcher) const override;

	};*/
}

#include "EntitiesGen.inl"