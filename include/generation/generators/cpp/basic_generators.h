#pragma once

#include <generation/interfaces/IGenerator.h>
#include <core/entities/cpp/basic_entites.h>

namespace cg {
    class NamedEntityGenerator : public IGenerator {
    public:
        std::string generate(
            const core::Node* ent,
            bool declaration = false,
            const core::Node* scoup = nullptr,
            const IGenerator* dispetcher = nullptr
        ) override;
    };
}