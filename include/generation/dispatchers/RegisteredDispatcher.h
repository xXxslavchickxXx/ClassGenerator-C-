#pragma once

#include <generation/interfaces/IDispatcher.h>
#include <generation/interfaces/IGenerator.h>
#include <core/base/typeRegistry/TypeRegistry.h>

#include <core/base/node/Node.h>

#include <format>

namespace cg {
    class RegisteredDispatcher :
    public core::TypeRegistry<IGenerator>,
    public IDispatcher
    {
    public:
        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr
        ) const override {
            for (const auto& [type, gen] : entities) {
                if (gen->can_generate(ent)) {
                    return gen->generate(ent, declaration, scoup, this);
                }
            }

            return "";
        }
    };

    /// @brief RegisteredDispatcherFabricator
    /// @tparam ...T 
    template<typename... T>
    using RegDispatcherFtor = TypeRegistryFabricator<RegisteredDispatcher, T...>;
}