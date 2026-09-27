#pragma once

#include <core/entities/EntityFabricator.h>
#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>

namespace cg::entities::cpp {
    template<typename... T>
    using Node = EntityFabricator<cg::core::Node, T...>;

    template<typename... T>
    using TreeNode = EntityFabricator<cg::core::TreeNode, T...>;

    using Namespace = TreeNode<NamedEntity, DefinitionEntity>;
    using Class = TreeNode<NamedEntity, DefinitionEntity>;
}