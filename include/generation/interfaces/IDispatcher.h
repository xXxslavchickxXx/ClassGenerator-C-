#pragma once

#include <string>

namespace cg {
    class IDispatcher {
    public:
        virtual std::string generate(
            const core::INode* ent,
            bool declaration = false,
            const core::INode* scoup = nullptr,
            const IDispatcher* = nullptr
        ) = 0;
    };
}