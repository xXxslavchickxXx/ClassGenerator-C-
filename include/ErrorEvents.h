#pragma once
#include <string>
#include <functional>
#include <unordered_map>
#include <concepts>

namespace pl::error {

    // 1. Enum для стратегий
    enum class ResolutionStrategy {
        Ignore,
        ApplyOptionA,
        ApplyOptionB,
        CustomAction
    };

    template<typename T>
    concept IsStrategy = requires {
        T::Ignore;
    };

    template<typename EnumType>
    struct ConflictContext {
        EnumType type;
        std::string message;
    };

    template<typename T, typename ErrorEnum>
    concept IsErrorContext = requires(T ctx, ErrorEnum e) {
        { ctx.type } -> std::convertible_to<ErrorEnum>;
    };

    template<typename ErrorEnum,
    typename ErrorContext = ConflictContext<ErrorEnum>,
    IsStrategy ErrorStrategy = ResolutionStrategy>
        requires IsErrorContext<ErrorContext, ErrorEnum>
    class ConflictResolver {
    public:
        using Resolver = std::function<ErrorStrategy(const ErrorContext&)>;

    private:
        std::unordered_map<ErrorEnum, Resolver> resolvers;
        Resolver default_resolver;

    public:
        ConflictResolver() = default;
        ConflictResolver(std::unordered_map<ErrorEnum, Resolver>&& map)
            : resolvers(std::forward<std::unordered_map<ErrorEnum, Resolver>>(map)) {}

        ConflictResolver(ConflictResolver&&) = default;
        ConflictResolver& operator=(ConflictResolver&&) = default;
        ConflictResolver(const ConflictResolver&) = default;
        ConflictResolver& operator=(const ConflictResolver&) = default;

        void register_resolver(ErrorEnum error, Resolver handler) {
            resolvers[error] = std::move(handler);
        }

        void set_default_resolver(Resolver handler) {
            default_resolver = std::move(handler);
        }

        ErrorStrategy resolve(const ErrorContext& ctx) {
            std::cout << "[Error] " << ctx.message << '\n';

            auto it = resolvers.find(ctx.type);
            if (it != resolvers.end()) {
                return it->second(ctx);
            }

            if (default_resolver) {
                return default_resolver(ctx);
            }

            return ErrorStrategy::Ignore;
        }
    };

}