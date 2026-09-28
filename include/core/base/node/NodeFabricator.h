#pragma once

#include <core/base/interfaces/IEntity.h>
#include <type_traits>
#include <tuple>

#include <core/base/concepts.h>

template<typename NT, typename... T>
class NodeFabricator : public NT {
    static_assert(
        ((std::is_base_of_v<this->entities.service_type, T>) && ...),
        "Every element in list would be derived from service_type"
    );

public:
    NodeFabricator() : NT() {
        ((this->entities.template add<T>()), ...);
    }
    
    template<typename... U>
        requires(sizeof...(U) > 0)
    NodeFabricator(U&&... ents) : NT() {
        static_assert(
            (is_one_of_v<U, T...> && ...),
            "Every element in list would be derived from IEntity"
        );

        // We add only the objects that have been passed.
        (this->entities.template add<U>(std::forward<U>(ents)), ...);

        // We are looking for entities that are not mentioned in 
        // the list of U, but are present in the list of T.
        ((!is_one_of_v<T, U...> ? this->entities.template add<T>() : 0), ...);
    }
};

template<typename... T>
using NodeFabrica = NodeFabricator<cg::core::Node, T...>;
template<typename... T>
using TreeNodeFabrica = NodeFabricator<cg::core::TreeNode, T...>;