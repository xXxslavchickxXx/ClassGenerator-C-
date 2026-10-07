#pragma once

#include <string>
#include <vector>
#include <core/macros.h>

#include <core/base/interfaces/IEntity.h>

namespace cg::core { class Node; }

namespace cg::entities::cpp {
	class TypeHandler : public cg::core::IEntity {
		const cg::core::Node* type;
	
	public:
		TypeHandler(const cg::core::Node* type = nullptr);

		const cg::core::Node* get_type() const;
		void set_type(const cg::core::Node* new_type);
	
	};

	/// @brief Just an entity that can store a 
	/// specific class, such as a type or template
	class ClassHandler : public cg::core::IEntity {
		const cg::core::Node* nested_class;
	
	public:
		ClassHandler(const cg::core::Node* nested_class = nullptr);

		const cg::core::Node* get_class() const;
		void set_class(const cg::core::Node* new_ref);
	};

    class NamedEntity : public cg::core::IEntity {
        std::string name;

    public:
        NamedEntity(const std::string& new_name = "");

        void set_name(const std::string& new_name);
        const std::string& get_name() const;
    };

	/// <summary>
	/// @brief По сути генерацию определения можно
	/// выделить как отдельный флаг, так как определения
	/// может вовсе не быть или быть, но под конкретный запрос
	/// под платформу или кроссплатформенный код
	/// </summary>
	enum class DEFINITION_TYPE : uint16_t {
		NO_DEFINITION = 0,
		CROSSPLATOFORM = 1 << 0,
		WINDOWS = 1 << 1,
		LINUX = 1 << 2,
		APPLE = 1 << 3
	};

	class DefinitionEntity : public cg::core::IEntity {
		DEFINITION_TYPE type;

	public:
		DefinitionEntity(
			const DEFINITION_TYPE& definition =
			DEFINITION_TYPE::NO_DEFINITION);

		const DEFINITION_TYPE& get_definition() const;
		DefinitionEntity& set_definition(const DEFINITION_TYPE& definition);

		bool has_definition() const;
	};
	
	enum class Access {
		PRIVATE,
		PROTECTED,
		PUBLIC
	};

	class AccessEntity : public cg::core::IEntity {
		Access access_ = Access::PRIVATE;

	public:
		AccessEntity(Access access = Access::PRIVATE);

		const Access& get_access() const;
		AccessEntity& set_access(const Access& access);
	};

    enum class TYPE_QUAL : uint16_t {
        NONE = 0,
        CONSTANT = 1 << 0,
        VOLATILE = 1 << 1
    };
    ENABLE_BITMASK_OPERATORS(TYPE_QUAL)

    class TypeQualificator : public cg::core::IEntity {
		TYPE_QUAL mask;

	public:
		TypeQualificator();

		bool is_const() const;
		TypeQualificator& toggle_const();

        bool is_volatile() const;
		TypeQualificator& toggle_volatile();

        TypeQualificator& set_mask(const TYPE_QUAL& _mask);
	};

	enum class TYPE_CONSTRUCTORS {
		NON_QUAL,
		POINTER,
		REFERENCE,
		UNIVERSAL_REFERENCE
	};

	class TypeConstructor : public TypeQualificator {
		TYPE_CONSTRUCTORS type;

	public:
		TypeConstructor();

		bool has_qualificator() const;
		const TYPE_CONSTRUCTORS& get_qualificator() const;

		TypeConstructor& set_qualificator(const TYPE_CONSTRUCTORS& new_qual);
		TypeConstructor& toggle_to_pointer();
		TypeConstructor& toggle_to_reference();
		TypeConstructor& toggle_to_uni_ref();
		TypeConstructor& reset_qualificator();

		TypeConstructor& toggle_const();
		TypeConstructor& toggle_volatile();
	};
}