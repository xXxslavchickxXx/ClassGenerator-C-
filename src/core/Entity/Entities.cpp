#include <Entity/Entities.h>

#include <algorithm>

namespace cg::ent {
    /*void Class::add_base_class(Node* raw,
        const src::Access& acs)
    {
        if (!raw) return;
        auto shared = raw->shared_from_this();
        base_classes.push_back({std::dynamic_pointer_cast<Class>(shared),
            acs});
    }

    const Class* Class::get_base_class(size_t where) const {
        if (where >= base_class_count()) return nullptr;

        return base_classes[where].first.get();
    }
    Class* Class::get_base_class(size_t where) {
        if (where >= base_class_count()) return nullptr;

        return base_classes[where].first.get();
    }
    const src::Access& Class::get_base_class_access(size_t where) const {
        if (where >= base_class_count()) throw std::runtime_error("out of range");

        return base_classes[where].second;
    }
    void Class::change_access(size_t where, const src::Access& acs) {
        if (where >= base_class_count()) return;

        base_classes[where].second = acs;
    }

    size_t Class::base_class_count() const {
        return base_classes.size();
    }
    void Class::erase_base_class(size_t where) {
        if (where >= base_classes.size()) return;

        base_classes.erase(base_classes.begin() + where);
    }
    void Class::swap_base_classes(size_t from, size_t to) {
        if (from >= size() || to >= size()) return;

        std::iter_swap(base_classes.begin() + from, base_classes.begin() + to);
    }*/
}