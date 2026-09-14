#include <iostream>

#include <core/base/node/Node.h>
#include <core/entities/basic_entites.h>
#include <string>

int main()
{
    // auto head = std::make_unique<cg::core::TreeNode>();
    // auto tail = std::make_unique<cg::core::Node>();
    
    // head->add_child(std::move(tail));

    // Пытаюсь создать переменную
    // auto var = std::make_unique<cg::core::EntityHandler>();


    // //tail->add(NamedEntity("some_node"));

    // head->get_children().front()->get_entities()->add<cg::entities::NamedEntity>();
    // auto nme = head->get_children().front()->get_entities()->get<cg::entities::NamedEntity>();
    // if (nme) {
    //     nme->set_name("some_name");
    //     std::cout << nme->get_name();
    // }
    std::cout << "Hello main";
    return 0;
}