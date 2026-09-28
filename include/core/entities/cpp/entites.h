#pragma once

#include <core/base/typeRegistry/TypeRegistryFabricator.h>
#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>

namespace cg::entities::cpp {
    template<typename... T>
    using NodeFabrica = TypeRegistryFabrica<cg::core::Node, T...>;
    template<typename... T>
    using TreeNodeFabrica = TypeRegistryFabrica<cg::core::TreeNode, T...>;

	// Классы заглушки
	class INamespace : public core::IEntity {};
    class IClass : public core::IEntity {};

    using Namespace = TreeNodeFabrica<NamedEntity, DefinitionEntity, INamespace>;
    using Class = TreeNodeFabrica<NamedEntity, DefinitionEntity, IClass>;
}