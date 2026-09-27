#pragma once

#include <generation/interfaces/IGenerator.h>

#include <map>
#include <memory>

namespace cg {
    class PriorityGenerator : public IGenerator {
        std::map<int, std::unique_ptr<IGenerator>> priority_map;
        
        // A high priority implies a smaller number than a low priority.
        // That is, the higher the priority, the smaller the number, since
        // generation occurs in ascending order.
        int lowest_priority = 0;

    public:
        template<typename T>
        IGenerator* create_gen(int priority) {
            static_assert(std::is_base_of_v<IGenerator, T>,
            "Creating generator would be base of IGenerator");

            return take(priority, std::make_unique<T>());
        }

        IGenerator* take(int priority, std::unique_ptr<IGenerator> gen);

        /// @brief the gen we want to add as a priority will be assigned
        /// to a new low_priority, which will be increased by 1.
        /// @param gen 
        /// @return raw ptr on added gen
        IGenerator* put_back(std::unique_ptr<IGenerator> gen);
        
        /// @brief The new generator will be inserted at the highest priority
        /// level, which means that all generators already in the list will
        /// have their priority increased by 1.
        ///
        /// @param gen 
        /// @return raw ptr on added gen
        IGenerator* put_front(std::unique_ptr<IGenerator> gen);

        std::unique_ptr<IGenerator> release(int priority);

    public:
        PriorityGenerator() = default;

        PriorityGenerator(PriorityGenerator&&) = default;
        PriorityGenerator& operator=(PriorityGenerator&&) = default;

        PriorityGenerator(const PriorityGenerator&) = delete;
        PriorityGenerator& operator=(const PriorityGenerator&) = delete;

        bool can_generate(const core::Node* node) const override;

        std::string generate(
            const core::Node* ent,
            bool declaration = false,
            const core::Node* scoup = nullptr,
            const IGenerator* dispetcher = nullptr
        ) override;
    };

    template<typename... T>
    struct PriorityGeneratorFabricator : public PriorityGenerator {
        static_assert((std::is_base_of_v<IGenerator, T> && ...),
            "Creating generator would be base of IGenerator");
        
        using PriorityGenerator::PriorityGenerator;

        PriorityGeneratorFabricator() {
            (put_back(std::make_unique<T>()), ...);
        }
    };
}