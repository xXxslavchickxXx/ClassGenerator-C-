#pragma once

#include <core/entities/EntityFabricator.h>
#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>

namespace cg::entities::cpp {
	// Классы заглушки
	class INamespace : public core::IEntity {};
    class IClass : public core::IEntity {};

    using Namespace = TreeNodeFabrica<NamedEntity, DefinitionEntity, INamespace>;
    using Class = TreeNodeFabrica<NamedEntity, DefinitionEntity, IClass>;
}