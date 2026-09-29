#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include <core/base/interfaces/IEntity.h>

namespace cg::core {
    template<typename Type>
    class TypeRegistry {
    protected:
        using service_type = Type;
        std::unordered_map<std::type_index, std::unique_ptr<Type>> entities;

    public:
        TypeRegistry() = default;

        TypeRegistry(const TypeRegistry&) = delete;
        TypeRegistry& operator=(const TypeRegistry&) = delete;
        TypeRegistry(TypeRegistry&&) = default;
        TypeRegistry& operator=(TypeRegistry&&) = default;

        template<typename T>
        std::remove_cvref_t<T>* add();
        template<typename T>
        std::remove_cvref_t<T>* add(T&& entity);
        template<typename T>
        T* take(std::unique_ptr<T> ent_ptr);

        template<typename T>
        void remove();

        template<typename T>
        std::unique_ptr<std::remove_cvref_t<T>> release();

        template<typename T>
        std::remove_cvref_t<T>* get();

        template<typename T>
        const std::remove_cvref_t<T>* get() const;

        template<typename T>
        bool has() const;
    };
}

#include "TypeRegistry.inl"
#include "TypeRegistryFabricator.inl"