#include <core/entities/cpp/entites.h>

#include <core/entities/cpp/basic_entites.h>

namespace cg::entities::cpp {
    Namespace::Namespace() : TreeNode() {
        this->entities.add<NamedEntity>();
    }
    Namespace::Namespace(Node* parent) : TreeNode(parent) {
        this->entities.add<NamedEntity>();
    }

}