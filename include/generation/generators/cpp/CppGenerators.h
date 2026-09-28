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
            bool declaration = false,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override;
    };
}