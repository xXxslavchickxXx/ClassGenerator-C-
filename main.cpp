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

    auto int_cls = std::make_shared<Class>("int");

    first->add_child(second);
    second->add_child(third);
    second->add_child(int_cls);
    
    auto registry = GeneratorRegistrator();
    registry.registry(std::make_unique<NamespaceGenerator>());
    registry.registry(std::make_unique<ClassGenerator>());

    auto int_t = Type(int_cls.get());



    try {
        //std::cout << registry.generate(second.get());
        std::cout << registry.generate(second.get());
    }
    catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }
}