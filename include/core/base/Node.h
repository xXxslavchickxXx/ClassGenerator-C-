#pragma once

#include <memory>
#include <vector>

#include <core/base/EntityHandler.h>

namespace cg::core {
    class Node :
        public EntityHandler
    {
        const Node* parent;

    public:
        Node();
        Node(const Node* parent);

        template<typename... Entities>
            requires (std::is_base_of_v<IEntity, std::remove_cvref_t<Entities>> && ...)
        Node(const Node* p, Entities&&... entities) : parent(p) {
            (add(std::forward<Entities>(entities)), ...);
        }
        template<typename... Entities>
            requires (sizeof...(Entities) > 0) &&
            (std::is_base_of_v<IEntity, std::remove_cvref_t<Entities>> && ...)
        Node(Entities&&... entities) : Node(nullptr, std::forward<Entities>(entities)...) {}

        const Node* get_parent() const;
        void set_parent(const Node* new_parent);
    };
}