#pragma once

#include <string>
#include <core/base/interfaces/INode.h>

namespace cg {
    class IGenerator {
    public:
        virtual std::string generate(
            const core::INode* ent,
            bool declaration = false,
            const core::INode* scoup = nullptr
        ) = 0;
    };
}