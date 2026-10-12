#include <core/entities/cpp/BasicEntities.h>

#include <core/base/node/Node.h>
#include <core/entities/cpp/EntitiyMarkers.h>

namespace cg::entities::cpp {
    ArgumentList::ArgumentList(std::initializer_list<const cg::core::Node*> arg_list) {
        for (auto* el : arg_list)
            if (!el->get_entities().has<cg::entities::cpp::IArgument>())
                throw std::runtime_error("all elements of arg list would derived from IArgument");
            
        for (auto* el : arg_list)
            arguments.push_back(el);
    }
}