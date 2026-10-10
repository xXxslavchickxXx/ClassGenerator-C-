#pragma once

#include "IGenerator.h"
#include <core/base/interfaces/IEntity.h>
#include <type_traits>
#include <core/base/node/Node.h>

namespace cg {

    /// @brief This class guarantees the generation of only those 
    /// nodes that contain a list of entities specified in the class template
    /// @tparam ...Entities This is a list of entities that must exist 
    /// in a node at the time of generation
    template<typename... Entities>
    class IEntityCheckedGenerator : public IGenerator {
        static_assert((std::is_base_of_v<core::IEntity, Entities> && ...),
        "all Entities would be derived from IEntity");

    public:
        std::string generate(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const override final {
            if (!ent || !(ent->get_entities().has<Entities>() && ...))
                throw std::runtime_error("the node entity list does not contain the necessary entities");

            return generate_impl(ent, declaration, scoup, dispatcher);
        }

    private:
        virtual std::string generate_impl(
            const core::Node* ent,
            bool declaration = true,
            const core::Node* scoup = nullptr,
            const IDispatcher* dispatcher = nullptr
        ) const = 0;

    };
}