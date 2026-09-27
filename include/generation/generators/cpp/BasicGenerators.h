#pragma once

#include <generation/interfaces/IGenerator.h>
#include <core/entities/cpp/BasicEntities.h>

namespace cg {
    class NamedEntityGenerator : public IGenerator {
    public:
        std::string generate(
            const core::Node* ent,
            bool declaration = false,
            const core::Node* scoup = nullptr,
            const IGenerator* dispatcher = nullptr
        ) override;
    };
}