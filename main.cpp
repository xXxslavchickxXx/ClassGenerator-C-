#include <iostream>

#include <Entity/Entities.h>
#include <Generator/MixinGen.h>
#include <Generator/EntitiesGen.h>

using namespace cg::src;
using namespace cg::ent;
using namespace cg::gen;

int main() {
    auto first = std::make_shared<Namespace>("first");
    auto second = std::make_shared<Namespace>("second");
    auto fake_second = std::make_shared<Namespace>("second");
    auto third = std::make_shared<Namespace>("third");

    auto cls_1 = std::make_shared<Class>("Classik");

    first->add_child(second);
    second->add_child(third);
    second->add_child(cls_1);
    
    auto registry = GeneratorRegistrator();
    registry.registry(std::make_unique<NamespaceGenerator>());

    std::cout << registry.generate(second.get());
}