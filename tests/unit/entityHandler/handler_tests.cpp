#include <gtest/gtest.h>

#include <core/base/entity/EntityHandler.h>
#include <string>

namespace tests {
    struct EntityA : public cg::core::IEntity {
        int value = 0;

        EntityA(int val = 0) : value(val) {}
    };
    
    struct EntityB : public cg::core::IEntity {
        std::string word;
    
        EntityB(const std::string& word_ = "") : word(word_) {}
    };
}

TEST(EntityHandler, DefaultCtor) {
    auto handler = cg::core::EntityHandler();

    EXPECT_EQ(handler.get_data().empty(), true);
}

TEST(EntityHandler, AddServiceWithoutData) {
    auto handler = cg::core::EntityHandler();

    handler.add<tests::EntityA>();

    EXPECT_EQ(handler.has<tests::EntityA>(), true);
    EXPECT_EQ(handler.has<tests::EntityB>(), false);
}

TEST(EntityHandler, GetService) {
    auto handler = cg::core::EntityHandler();

    auto* ptr = handler.add<tests::EntityA>();

    EXPECT_EQ(handler.get<tests::EntityA>(), ptr);
}

TEST(EntityHandler, AddServiceWithData) {
    auto handler = cg::core::EntityHandler();

    int validate_num = 6;
    handler.add(tests::EntityA(validate_num));

    EXPECT_EQ(handler.get<tests::EntityA>()->value, validate_num);
}

TEST(EntityHandler, AddPtrService) {
    auto handler = cg::core::EntityHandler();

    int validate_num = 6;
    handler.add(std::make_unique<tests::EntityA>(validate_num));

    EXPECT_EQ(handler.get<tests::EntityA>()->value, validate_num);
}

TEST(EntityHandler, RemoveService) {
    auto handler = cg::core::EntityHandler();

    handler.add(tests::EntityA());
    handler.remove<tests::EntityA>();

    EXPECT_EQ(handler.has<tests::EntityA>(), false);
}

TEST(EntityHandler, MoveService) {
    auto handler = cg::core::EntityHandler();
    
    int validate_num = 6;
    handler.add(tests::EntityA(validate_num));

    auto serv = handler.move_service<tests::EntityA>();

    EXPECT_EQ(handler.has<tests::EntityA>(), false);
    EXPECT_EQ(serv->value, validate_num);
}

TEST(EntityHandler, addExistService) {
    auto handler = cg::core::EntityHandler();

    int validate_num = 6;
    handler.add(tests::EntityA(validate_num));

    handler.add(tests::EntityA());

    EXPECT_EQ(handler.get<tests::EntityA>()->value, validate_num);
}

TEST(EntityHandler, RemoveNonExistService) {
    auto handler = cg::core::EntityHandler();

    handler.remove<tests::EntityA>();

    SUCCEED();
}

TEST(EntityHandler, MoveNonExistService) {
    auto handler = cg::core::EntityHandler();

    auto serv = handler.move_service<tests::EntityA>();

    EXPECT_EQ(serv, nullptr);
}