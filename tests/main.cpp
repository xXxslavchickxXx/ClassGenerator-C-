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
    NamedEntity(const std::string& new_name = "") : name(new_name) {}

    void set_name(const std::string& new_name) { name = new_name; }
    const std::string& get_name() const { return name; }
};

int main()
{
    auto head = std::make_shared<cg::core::TreeNode>();
    auto tail = std::make_shared<cg::core::Node>();

    //tail->add(NamedEntity("some_node"));

    head->add_child(std::move(tail));
    head->get_children().front()->add<NamedEntity>();
    auto nme = head->get_children().front()->get<NamedEntity>();
    if (nme) {
        nme->set_name("some_name");
        std::cout << nme->get_name();
    }
}