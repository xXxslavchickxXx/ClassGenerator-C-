#include <Validate/Validate.h>
#include <Entity/Entities.h>
#include <iostream>

namespace cg::verif {
    void validate_namespace(const std::shared_ptr<ent::Namespace>& ns) {
        for (size_t i = 0; i < ns->size(); ++i) {
            auto* child = ns->get_child(i);
            if (!child) continue;

            if (auto named = child->as<const src::NamedEntity>()) {
                if (child->as<const ent::Field>() ||
                    child->as<const ent::Method>())
                        throw ValidationError(
                            "Entity '" + named->get_name() +
                            "' cannot exist in namespace"
                        );
            }
        }
    }
}