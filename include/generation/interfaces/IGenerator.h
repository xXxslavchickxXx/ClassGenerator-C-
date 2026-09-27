#pragma once

#include <string>

namespace cg::core { class Node; }

namespace cg {
    class IGenerator {
    public:
        virtual bool can_generate(const core::Node* node) const { return true; }

        virtual std::string generate(
            const core::Node* ent,
            bool declaration = false,
            const core::Node* scoup = nullptr,
            const IGenerator* dispatcher = nullptr
        ) = 0;

        virtual ~IGenerator() = default;
    };
}