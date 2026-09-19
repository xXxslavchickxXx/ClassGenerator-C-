#include <gtest/gtest.h>

#include <core/base/node/Node.h>
#include <core/entities/cpp/basic_entites.h>
#include <string>

namespace tests {
    constexpr int default_value = 0;
    struct EntityA : public cg::core::IEntity {
        int value;

        EntityA(int val = default_value) : value(val) {}

        ~EntityA() = default;
    };
}

TEST(EntityHandlerIntegrationTest, NodeMoveService) {
    // Creating 2 diffenet nodes;
    auto node_1 = cg::core::Node();
    auto node_2 = cg::core::Node();

    int test_value = 10;

    // Add service to first node
    auto* service = node_1.get_entities().add<tests::EntityA>();
    
    // Set testing value to service in first node
    service->value = test_value;

    // We’re checking that the service is really unavailable.
    EXPECT_EQ(node_2.get_entities().get<tests::EntityA>(), nullptr);

    // Moving service
    node_2.get_entities().take(node_1.get_entities().move_service<tests::EntityA>());

    // We verify that the transferred service has
    // retained its initial value and that the service was transferred correctly.
    EXPECT_EQ(node_2.get_entities().get<tests::EntityA>()->value, test_value);
    EXPECT_EQ(node_1.get_entities().get<tests::EntityA>(), nullptr);
}