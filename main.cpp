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

    //auto root = Class("Class");

    std::cout << "hello class generator";

    return 0;
}