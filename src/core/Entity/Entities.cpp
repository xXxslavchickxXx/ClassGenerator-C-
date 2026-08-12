#include <Entity/Entities.h>

#include <algorithm>

namespace cg::ent {
    void Class::add_base_class(Node* raw) {
        if (!raw) return;
        auto shared = raw->shared_from_this();
        base_classes.push_back(std::dynamic_pointer_cast<Class>(shared));
    }
    void Class::erase_base_class(size_t where) {
        if (where >= base_classes.size()) return;

        base_classes.erase(base_classes.begin() + where);
    }
    void Class::swap_base_classes(size_t from, size_t to) {
        if (from >= size() || to >= size()) return;

        std::iter_swap(base_classes.begin() + from, base_classes.begin() + to);
    }
}