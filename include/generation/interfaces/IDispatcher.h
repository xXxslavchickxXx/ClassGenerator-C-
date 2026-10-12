#pragma once

#include <string>

namespace cg::core { class TreeNodeBase; }

namespace cg {
    class IDispatcher {
    public:
        virtual std::string generate(
            const core::TreeNodeBase* ent,
            bool declaration = false,
            const core::TreeNodeBase* scope = nullptr
        ) const = 0;

        virtual ~IDispatcher() = default;
    };
}