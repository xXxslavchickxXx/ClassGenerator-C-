#pragma once

#include <string>

namespace cg::core { class Node; }

namespace cg {
    class IDispatcher {
    public:
        virtual std::string generate(
            const core::Node* ent,
            bool declaration = false,
            const core::Node* scoup = nullptr
        ) const = 0;

        virtual ~IDispatcher() = default;
    };
}