#pragma once

#include <generation/interfaces/IDispatcher.h>
#include <string>

namespace cg::core { class TreeNodeBase; }

namespace cg {
    class IGenerator {
    public:
        virtual bool can_generate(const core::TreeNodeBase* node) const { return true; }

        virtual std::string generate(
            const core::TreeNodeBase* ent,
            bool declaration = false,
            const core::TreeNodeBase* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const = 0;

        virtual ~IGenerator() = default;
    };
}