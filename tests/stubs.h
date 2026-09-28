#pragma once

#include <core/entities/EntityFabricator.h>
#include <core/base/node/Node.h>
#include <generation/generators/PriorityGenerator.h>

#include <string>

namespace tests {
    /// @brief Stub class with integer num
    struct EntityA : public cg::core::IEntity {
        int value = 0;

        EntityA(int val = 0) : value(val) {}

        ~EntityA() = default;
    };
    
    /// @brief Stub class with word
    struct EntityB : public cg::core::IEntity {
        std::string word;
    
        EntityB(const std::string& word_ = "") : word(word_) {}
        ~EntityB() = default;
    };

    struct AGen : public cg::IGenerator {
        std::string generate(
            const cg::core::Node* ent,
            bool declaration = false,
            const cg::core::Node* scoup = nullptr,
            const cg::IDispatcher* dispetcher = nullptr
        ) const override {
            if (!ent->get_entities().has<EntityA>()) return "";

            return std::to_string(ent->get_entities().get<EntityA>()->value);
        }
    };

    struct BGen : public cg::IGenerator {
        std::string generate(
            const cg::core::Node* ent,
            bool declaration = false,
            const cg::core::Node* scoup = nullptr,
            const cg::IDispatcher* dispetcher = nullptr
        ) const override {
            if (!ent->get_entities().has<EntityB>()) return "";

            return (ent->get_entities().get<EntityB>()->word);            
        }
    };
}