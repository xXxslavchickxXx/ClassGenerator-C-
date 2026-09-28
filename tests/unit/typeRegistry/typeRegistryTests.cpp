#include <gtest/gtest.h>
#include <stubs.h>

TEST(EntityHandler, DefaultCtor) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    SUCCEED();
}

TEST(EntityHandler, AddServiceWithoutData) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    handler.add<tests::EntityA>();

    EXPECT_EQ(handler.has<tests::EntityA>(), true);
    EXPECT_EQ(handler.has<tests::EntityB>(), false);
}

TEST(EntityHandler, GetService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    auto* ptr = handler.add<tests::EntityA>();

    EXPECT_EQ(handler.get<tests::EntityA>(), ptr);
}

TEST(EntityHandler, AddServiceWithData) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    int validate_num = 6;
    handler.add(tests::EntityA(validate_num));

    EXPECT_EQ(handler.get<tests::EntityA>()->value, validate_num);
}

TEST(EntityHandler, AddPtrService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    int validate_num = 6;
    handler.take(std::make_unique<tests::EntityA>(validate_num));

    EXPECT_EQ(handler.get<tests::EntityA>()->value, validate_num);
}

TEST(EntityHandler, RemoveService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    handler.add(tests::EntityA());
    handler.remove<tests::EntityA>();

    EXPECT_EQ(handler.has<tests::EntityA>(), false);
}

TEST(EntityHandler, MoveService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();
    
    int validate_num = 6;
    handler.add(tests::EntityA(validate_num));

    auto serv = handler.release<tests::EntityA>();

    EXPECT_EQ(handler.has<tests::EntityA>(), false);
    EXPECT_EQ(serv->value, validate_num);
}

TEST(EntityHandler, addExistService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    int validate_num = 6;
    handler.add(tests::EntityA(validate_num));

    EXPECT_ANY_THROW(handler.add(tests::EntityA()));
}

TEST(EntityHandler, RemoveNonExistService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    handler.remove<tests::EntityA>();

    SUCCEED();
}

TEST(EntityHandler, MoveNonExistService) {
    auto handler = cg::core::TypeRegistry<cg::core::IEntity>();

    auto serv = handler.release<tests::EntityA>();

    EXPECT_EQ(serv, nullptr);
}