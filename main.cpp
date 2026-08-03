#ifdef _WIN32
    #include <Windows.h>
#endif // WIN32__

#include <iostream>
#include <DefaultEntity/Node.h>

using namespace cg::source;

int main() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif // _WIN32

    auto root = NodeEntity<NodeType::CLASS>();

    std::cout << "hello class generator";

    return 0;
}