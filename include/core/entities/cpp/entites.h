#pragma once

#include <core/entities/cpp/BasicEntities.h>
#include <core/base/node/Node.h>
#include <core/entities/cpp/EntitiyMarkers.h>

namespace cg::entities::cpp {
    using Namespace = TreeNodeFabrica<NamedEntity, INamespace>;
    using Class = TreeNodeFabrica<NamedEntity, IClass>;
    using Type = TreeNodeFabrica<ClassHandler, TypeQualificator, TypeConstructor, IType>;
    using Alias = TreeNodeFabrica<NamedEntity, TypeHandler, IAlias>;
    using Variable = TreeNodeFabrica<NamedEntity, TypeHandler, IVariable>;
}