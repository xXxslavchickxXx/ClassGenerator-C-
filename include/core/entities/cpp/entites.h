#pragma once

#include <core/entities/EntityFabricator.h>
#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>

namespace cg::entities::cpp {
    using Namespace = TreeNodeFabrica<NamedEntity, DefinitionEntity>;
    using Class = TreeNodeFabrica<NamedEntity, DefinitionEntity>;
}