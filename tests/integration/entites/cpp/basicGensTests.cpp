#include <gtest/gtest.h>

#include <generation/generators/cpp/BasicGenerators.h>
#include <generation/generators/cpp/PriorityGenerator.h>

#include <core/base/node/Node.h>
#include <core/entities/cpp/BasicEntites.h>
#include <core/entities/EntityFabricator.h>

namespace tests {
    struct EntityA : public cg::core::IEntity {
        int value = 0;

        EntityA(int val = 0) : value(val) {}

        ~EntityA() = default;
    };
    
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
            const IGenerator* dispetcher = nullptr
        ) override {
            if (!ent->get_entities().has<EntityA>()) return "";

            return std::to_string(ent->get_entities().get<EntityA>()->value);
        }
    };

    struct BGen : public cg::IGenerator {
        std::string generate(
            const cg::core::Node* ent,
            bool declaration = false,
            const cg::core::Node* scoup = nullptr,
            const IGenerator* dispetcher = nullptr
        ) override {
            if (!ent->get_entities().has<EntityB>()) return "";

            return (ent->get_entities().get<EntityB>()->word);            
        }
    };
}

TEST(PriorityGenIntegrationTest, DeleteGenerator) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    auto gen = cg::PriorityGenerator();

    gen.take(1, std::make_unique<tests::AGen>());
    gen.take(2, std::make_unique<tests::BGen>());

    gen.release(2);

    EXPECT_EQ(gen.generate(node.get()), "5");
}

TEST(PriorityGenIntegrationTest, GenerateNestedGen) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    using NestedGenerator = cg::PriorityGeneratorFabricator<tests::BGen, tests::AGen>;

    auto gen = cg::PriorityGeneratorFabricator<NestedGenerator, tests::BGen>();

    EXPECT_EQ(gen.generate(node.get()), "word 5 word");
}

TEST(PriorityGenIntegrationTest, GeneratePutFront) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    auto gen = cg::PriorityGenerator();

    gen.put_front(std::make_unique<tests::AGen>());
    gen.put_front(std::make_unique<tests::BGen>());

    EXPECT_EQ(gen.generate(node.get()), "word 5");
}

TEST(PriorityGenIntegrationTest, GeneratePutBack) {
    // Test data
    std::string test_name = "word";
    int test_num = 5;

    // Entites with data
    auto ent_num = tests::EntityA(test_num);
    auto ent_word = tests::EntityB(test_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, tests::EntityA, tests::EntityB>>(ent_num, ent_word);

    auto gen = cg::PriorityGenerator();

    gen.put_back(std::make_unique<tests::AGen>());
    gen.put_back(std::make_unique<tests::BGen>());

    EXPECT_EQ(gen.generate(node.get()), "5 word");
}

TEST(NamedEntityGeneratorIntegrationTest, ReturnsEmptyWhenNoEntity) {
    cg::core::Node node;
    cg::NamedEntityGenerator gen;
    EXPECT_EQ(gen.generate(&node), "");
}

TEST(NamedEntityGeneratorIntegrationTest, GenerateNameFromEntity) {
    std::string node_name = "word";

    auto ent = cg::entities::cpp::NamedEntity(node_name);

    auto node = std::make_unique<EntityFabricator<cg::core::Node, cg::entities::cpp::NamedEntity>>(ent);

    auto gen = cg::NamedEntityGenerator();

    EXPECT_EQ(gen.generate(node.get()), node_name);
}