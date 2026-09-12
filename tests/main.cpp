#include <iostream>
#include <gtest/gtest.h>

#include <core/base/Node.h>
#include <core/base/IEntity.h>
#include <string>

TEST(someShit, some) {
    EXPECT_EQ(5, 5);
}

class NamedEntity : public cg::core::IEntity {
    std::string name;

public:
    NamedEntity(const std::string& new_name) : name(new_name) {}

    void set_name(const std::string& new_name) { name = new_name; }
    const std::string& get_name() const { return name; }
};

int main()
{
    auto head = std::make_unique<cg::core::Node>();
    auto tail = std::make_unique<cg::core::Node>(head.get());

    tail->add(NamedEntity("some_node"));
    //tail->add<NamedEntity>();

    std::cout << tail->get<NamedEntity>()->get_name();
    return 0;
}