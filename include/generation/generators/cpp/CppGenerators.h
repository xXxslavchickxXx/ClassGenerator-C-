#pragma once

#include <generation/interfaces/IEntityCheckedGenerator.h>
#include <core/entities/cpp/entites.h>

namespace cg::gen::cpp {
    std::string relative_path(
        const core::TreeNodeBase* ent,
        const core::TreeNodeBase* scope = nullptr
    );

    class FunctionGenerator :
    public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity
    > {
    public:
        bool can_generate(const core::TreeNodeBase* node) const override;

    private:
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class ArgumentGenerator :
    public IEntityCheckedGenerator<
        cg::entities::cpp::TypeHandler,
        cg::entities::cpp::ValueHandler,
        cg::entities::cpp::NamedEntity
    > {
    public:
        bool can_generate(const core::TreeNodeBase* node) const override;

    private:
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class NamespaceGenerator :
    public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity
    > {
    public:
        bool can_generate(const core::TreeNodeBase* node) const override;

    private:
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class ClassGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity
    > {
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
        
    public:
        bool can_generate(const core::TreeNodeBase* node) const override;

    };

    class TypeGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::ClassHandler,
        cg::entities::cpp::TypeQualificator,
        cg::entities::cpp::TypeConstructor
    > {
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;

    public:
        bool can_generate(const core::TreeNodeBase* node) const override;
    };

    class AliasGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity,
        cg::entities::cpp::TypeHandler
    > {
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;

    public:
        bool can_generate(const core::TreeNodeBase* node) const override;
    };

    class VariableGenerator : public IEntityCheckedGenerator<
        cg::entities::cpp::NamedEntity,
        cg::entities::cpp::TypeHandler,
        cg::entities::cpp::ValueHandler,
        cg::entities::cpp::DefinitionEntity,
        cg::entities::cpp::OptionalEntity
    > {
        std::string generate_impl(
            const core::TreeNodeBase* ent,
            bool declaration = true,
            const core::TreeNodeBase* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;

    public:
        bool can_generate(const core::TreeNodeBase* node) const override;
    };
}