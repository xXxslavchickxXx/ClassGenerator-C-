#pragma once

#include <core/base/interfaces/IEntity.h>
#include <type_traits>
#include <tuple>

namespace cg::core { class Node; }

template<typename U, typename... T>
constexpr bool is_one_of_v = (std::is_same_v<std::remove_cvref_t<U>, std::remove_cvref_t<T>> || ...);

template<typename NT, typename... T>
class EntityFabricator : public NT {
    static_assert(
        ((std::is_base_of_v<cg::core::IEntity, T>) && ...),
        "Every element in list would be derived from IEntity"
    );
    static_assert(
        std::is_base_of_v<cg::core::Node, NT>,
        "NT would derived from Node"
    );

public:
    EntityFabricator() : NT() {
        ((NT::entities.template add<T>()), ...);
    }
    
    template<typename... U>
        requires(sizeof...(U) > 0)
    EntityFabricator(U&&... ents) : NT() {
        static_assert(
            (is_one_of_v<U, T...> && ...),
            "Every element in list would be derived from IEntity"
        );

        // We add only the objects that have been passed.
        (NT::entities.template add<U>(std::forward<U>(ents)), ...);

        // We are looking for entities that are not mentioned in 
        // the list of U, but are present in the list of T.
        ((!is_one_of_v<T, U...> ? NT::entities.template add<T>() : 0), ...);
    }
};