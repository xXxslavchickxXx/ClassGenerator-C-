#include <stdexcept>

namespace cg::core {
    template<typename T>
    void EntityHandler::remove() {
        using U = std::remove_cvref_t<T>;
        
        auto it = entities.find(std::type_index(typeid(U)));

        if (it != entities.end()) entities.erase(it);
    }
    
    template<typename T>
    T* EntityHandler::take(std::unique_ptr<T> ent_ptr) {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<IEntity, U>,
        "T must derive from IEntity");

        if (has<U>()) throw std::runtime_error("this service already exist, data can disappear");
        
        entities[std::type_index(typeid(U))] = std::move(ent_ptr);

        return static_cast<U*>(entities[std::type_index(typeid(U))].get());
    }
    
    template<typename T>
    std::unique_ptr<std::remove_cvref_t<T>> EntityHandler::move_service() {
        using U = std::remove_cvref_t<T>;
        if (!has<U>()) return nullptr;

        auto it = entities.find(std::type_index(typeid(U)));

        U* temp = static_cast<U*>(it->second.release());

        entities.erase(it);

        return std::unique_ptr<U>(temp);
    }

    template<typename T>
    std::remove_cvref_t<T>* EntityHandler::add() {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<IEntity, U>,
        "T must derive from IEntity");
        static_assert(std::is_default_constructible_v<U>,
        "Type doesn't have default constructor");

        if (!has<U>()) entities[std::type_index(typeid(U))] = std::make_unique<U>();

        return static_cast<U*>(entities[std::type_index(typeid(U))].get());
    }
    template<typename T>
    std::remove_cvref_t<T>* EntityHandler::add(T&& entity) {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<IEntity, U>,
        "T must derive from IEntity");

        if (!has<U>()) entities[std::type_index(typeid(U))] = std::make_unique<U>(std::forward<T>(entity));

        return static_cast<U*>(entities[std::type_index(typeid(U))].get()); 
    }

    template<typename T>
    std::remove_cvref_t<T>* EntityHandler::get() {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<IEntity, U>, "T must derive from IEntity");
        auto it = entities.find(std::type_index(typeid(U)));
        return it != entities.end() ? static_cast<U*>(it->second.get()) : nullptr;
    }

    template<typename T>
    const std::remove_cvref_t<T>* EntityHandler::get() const {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<IEntity, U>, "T must derive from IEntity");
        auto it = entities.find(std::type_index(typeid(U)));
        return it != entities.end() ? static_cast<const U*>(it->second.get()) : nullptr;
    }
    template<typename T>
    bool EntityHandler::has() const {
        return
            entities.find(
                std::type_index(typeid(std::remove_cvref_t<T>))
            )
                !=
            
            entities.end();
    }    

}