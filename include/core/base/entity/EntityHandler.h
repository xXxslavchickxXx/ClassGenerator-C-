#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <core/base/interfaces/IEntity.h>

namespace cg::core {

    class EntityHandler {
        std::unordered_map<std::type_index, std::unique_ptr<IEntity>> entities;

    public:
        EntityHandler() = default;

        EntityHandler(const EntityHandler&) = delete;
        EntityHandler& operator=(const EntityHandler&) = delete;
        EntityHandler(EntityHandler&&) = default;
        EntityHandler& operator=(EntityHandler&&) = default;

        template<typename T>
        std::remove_cvref_t<T>* add();
        template<typename T>
        std::remove_cvref_t<T>* add(T&& entity);
        template<typename T>
        T* take(std::unique_ptr<T> ent_ptr);

        template<typename T>
        void remove();

        template<typename T>
        std::unique_ptr<std::remove_cvref_t<T>> move_service();

        template<typename T>
        std::remove_cvref_t<T>* get();

        template<typename T>
        const std::remove_cvref_t<T>* get() const;

        template<typename T>
        bool has() const;
    };
}

#include "EntityHandler.inl"