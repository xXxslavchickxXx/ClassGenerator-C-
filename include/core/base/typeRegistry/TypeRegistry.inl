#pragma once

#include <stdexcept>

namespace cg::core {
    template<typename Type>
    template<typename T>
    void TypeRegistry<Type>::remove() {
        using U = std::remove_cvref_t<T>;
        
        auto it = entities.find(std::type_index(typeid(U)));

        if (it != entities.end()) entities.erase(it);
    }
    
    template<typename Type>
    template<typename T>
    T* TypeRegistry<Type>::take(std::unique_ptr<T> ent_ptr) {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>,
        "T must derive from Type");

        if (has<U>()) throw std::runtime_error("this service already exist, data can disappear");
        if (ent_ptr.get() == nullptr) throw std::runtime_error("service can't be empty");
        
        entities[std::type_index(typeid(U))] = std::move(ent_ptr);

        return static_cast<U*>(entities[std::type_index(typeid(U))].get());
    }
    
    template<typename Type>
    template<typename T>
    std::unique_ptr<std::remove_cvref_t<T>> TypeRegistry<Type>::release() {
        using U = std::remove_cvref_t<T>;
        if (!has<U>()) return nullptr;

        auto it = entities.find(std::type_index(typeid(U)));

        U* temp = static_cast<U*>(it->second.release());

        entities.erase(it);

        return std::unique_ptr<U>(temp);
    }

    template<typename Type>
    template<typename T>
    std::remove_cvref_t<T>* TypeRegistry<Type>::add() {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>,
        "T must derive from Type");
        static_assert(std::is_default_constructible_v<U>,
        "Type doesn't have default constructor");

        if (!has<U>()) entities[std::type_index(typeid(U))] = std::make_unique<U>();
        else throw std::runtime_error("this service already exist, data can disappear");

        return static_cast<U*>(entities[std::type_index(typeid(U))].get());
    }
    template<typename Type>
    template<typename T>
    std::remove_cvref_t<T>* TypeRegistry<Type>::add(T&& entity) {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>,
        "T must derive from Type");

        if (!has<U>()) entities[std::type_index(typeid(U))] = std::make_unique<U>(std::forward<T>(entity));
        else throw std::runtime_error("this service already exist, data can disappear");

        return static_cast<U*>(entities[std::type_index(typeid(U))].get()); 
    }
    template<typename Type>
    template<typename T>
    std::remove_cvref_t<T>* TypeRegistry<Type>::replace() {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>,
        "T must derive from Type");
        static_assert(std::is_default_constructible_v<U>,
        "Type doesn't have default constructor");

        entities[std::type_index(typeid(U))] = std::make_unique<U>();
        
        return static_cast<U*>(entities[std::type_index(typeid(U))].get());
    }
    template<typename Type>
    template<typename T>
    std::remove_cvref_t<T>* TypeRegistry<Type>::replace(T&& entity) {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>,
        "T must derive from Type");

        entities[std::type_index(typeid(U))] = std::make_unique<U>(std::forward<T>(entity));
        
        return static_cast<U*>(entities[std::type_index(typeid(U))].get()); 
    }

    template<typename Type>
    template<typename T>
    std::remove_cvref_t<T>* TypeRegistry<Type>::get() {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>, "T must derive from Type");
        auto it = entities.find(std::type_index(typeid(U)));
        return it != entities.end() ? static_cast<U*>(it->second.get()) : nullptr;
    }

    template<typename Type>
    template<typename T>
    const std::remove_cvref_t<T>* TypeRegistry<Type>::get() const {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<Type, U>, "T must derive from Type");
        auto it = entities.find(std::type_index(typeid(U)));
        return it != entities.end() ? static_cast<const U*>(it->second.get()) : nullptr;
    }
    template<typename Type>
    template<typename T>
    bool TypeRegistry<Type>::has() const {
        return
            entities.find(
                std::type_index(typeid(std::remove_cvref_t<T>))
            )
                !=
            
            entities.end();
    }    

}