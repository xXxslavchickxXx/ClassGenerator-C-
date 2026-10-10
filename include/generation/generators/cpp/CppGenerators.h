#pragma once

#include <generation/interfaces/IEntityCheckedGenerator.h>
#include <core/entities/cpp/entites.h>

namespace cg::gen::cpp {
    std::string relative_path(
        const core::Node* ent,
        const core::Node* scope = nullptr
    );

    class NamespaceGenerator :
    public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity
    > {
    public:
        bool can_generate(const core::Node* node) const override;

    private:
        std::string generate_impl(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class ClassGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity
    > {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate_impl(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class TypeGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::ClassHandler,
        cg::entities::cpp::TypeQualificator,
        cg::entities::cpp::TypeConstructor
    > {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate_impl(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class AliasGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity,
        cg::entities::cpp::TypeHandler
    > {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate_impl(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class VariableGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity,
        cg::entities::cpp::TypeHandler,
        cg::entities::cpp::ValueHandler,
        cg::entities::cpp::DefinitionEntity,
        cg::entities::cpp::OptionalEntity
    > {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate_impl(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };
}