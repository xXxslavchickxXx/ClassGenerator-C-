#pragma once

#include <generation/interfaces/IGenerator.h>
#include <core/entities/cpp/entites.h>

namespace cg::gen::cpp {
    std::string relative_path(
        const core::Node* ent,
        const core::Node* scope = nullptr
    );

    class NamespaceGenerator : public IGenerator {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class ClassGenerator : public IGenerator {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class TypeGenerator : public IGenerator {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class AliasGenerator : public IGenerator {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };

    class VariableGenerator : public IGenerator {
    public:
        bool can_generate(const core::Node* node) const override;

        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scope = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };
}