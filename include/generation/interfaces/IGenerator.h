#pragma once

#include <generation/interfaces/IDispatcher.h>
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
            const IDispatcher* dispatcher = nullptr
        ) const = 0;

        virtual ~IGenerator() = default;
    };
}