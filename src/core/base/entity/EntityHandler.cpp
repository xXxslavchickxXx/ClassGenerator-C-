#include <core/base/entity/EntityHandler.h>

namespace cg::core {
    EntityHandler EntityHandler::clone() const {
        auto dummy = EntityHandler();

        for (auto& [idx, serv] : this->entities) {
            dummy.add(*serv);
        }

        return std::move(dummy);
    }
}