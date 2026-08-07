#ifdef _WIN32
    #include <Windows.h>
#endif // WIN32__

#include <iostream>
#include <Entity/AccessEntities.h>

using namespace cg::source;

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif // _WIN32

    auto root = Class::create("Class");

    auto int_cls = Class::create("int");

    auto T_t = TemplateArguement::create("T");
    auto U_t = TemplateArguement::create("U");

    root->add_template(T_t);
    root->add_template(U_t);

    root->get_template("T")->instance(int_cls);

    std::cout << "hello class generator";

    return 0;
}