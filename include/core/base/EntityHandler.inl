namespace cg::core {
    template<typename... Types>
        requires (std::is_base_of_v<IEntity, std::remove_cvref_t<Types>> && ...)
    EntityHandler::EntityHandler() {
        ((add<Types>()), ...);
    }
    template<typename... Args>
        requires (sizeof...(Args) > 0) &&
            (std::is_base_of_v<IEntity, std::remove_cvref_t<Args>> && ...)
    EntityHandler::EntityHandler(Args&&... args) {
        ((std::forward<Args>(add(args))), ...);
    }

    template<typename T>
    std::remove_cvref_t<T>* EntityHandler::add() {
        using U = std::remove_cvref_t<T>;
        static_assert(std::is_base_of_v<IEntity, U>,
        "T must derive from IEntity");
        static_assert(!std::is_abstract_v<U>,
        "Cannot instantiate abstract class");
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
        static_assert(!std::is_abstract_v<U>,
        "Cannot instantiate abstract class");

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
                std::type_index(typeid(T))
            )
                !=
            
            entities.end();
    }    

}