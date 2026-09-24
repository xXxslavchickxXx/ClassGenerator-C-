#pragma once

#include <core/entities/EntityFabricator.h>
#include <core/entities/cpp/basic_entites.h>
#include <core/base/node/Node.h>

namespace cg::entities::cpp {
    using Namespace = EntityFabricator<cg::core::TreeNode, NamedEntity>;
}