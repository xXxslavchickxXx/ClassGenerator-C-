#pragma once

#include <string>

namespace cg::core { class Node; }

namespace cg {
    class IGenerator {
    public:
        virtual std::string generate(
            const core::Node* ent,
            bool declaration = false,
            const core::Node* scoup = nullptr,
            const IGenerator* dispetcher = nullptr
        ) = 0;
    };
}