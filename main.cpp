#include <iostream>

#include <Entity/Entities.h>
#include <Generator/MixinGen.h>
#include <Generator/EntitiesGen.h>

using namespace cg::src;
using namespace cg::ent;
using namespace cg::gen;

int main() {
    auto global_ns = std::make_shared<Namespace>("first");
    auto ns_1 = std::make_shared<Namespace>("second");
    auto fake_ns_1 = std::make_shared<Namespace>("second");
    auto ns_2 = std::make_shared<Namespace>("third");

    global_ns->add_child(ns_1);
    ns_1->add_child(ns_2);

    std::shared_ptr<Node> sa = global_ns->get_child(0)->shared_from_this();
    
    std::cout << generate(ns_1.get()) << std::endl;
}