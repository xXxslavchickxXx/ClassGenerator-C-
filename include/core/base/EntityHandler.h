#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <core/base/IEntity.h>

namespace cg::core {

    class EntityHandler {
        std::unordered_map<std::type_index, std::unique_ptr<IEntity>> entities;

    public:
        template<typename... Types>
        EntityHandler();
        template<typename... Args>
        EntityHandler(Args&&... args);

        template<typename T>
        std::remove_cvref_t<T>* add();
        template<typename T>
        std::remove_cvref_t<T>* add(T&& entity);

        template<typename T>
        std::remove_cvref_t<T>* get();

        template<typename T>
        const std::remove_cvref_t<T>* get() const;

    private:
        template<typename T>
        bool has() const;

    };
}

#include "EntityHandler.inl"